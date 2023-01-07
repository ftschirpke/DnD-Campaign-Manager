#include "dnd_config.hpp"

#include "content_parser.hpp"

#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "controllers/content.hpp"
#include "parsing/content_file_parser.hpp"
#include "parsing/models/character_class_file_parser.hpp"
#include "parsing/models/character_file_parser.hpp"
#include "parsing/models/character_race_file_parser.hpp"
#include "parsing/models/character_subclass_file_parser.hpp"
#include "parsing/models/character_subrace_file_parser.hpp"
#include "parsing/models/spells_file_parser.hpp"
#include "parsing/parsing_exceptions.hpp"
#include "parsing/parsing_types.hpp"

dnd::Content dnd::ContentParser::parse(
    const std::filesystem::path& content_path, const std::string& campaign_dir_name
) {
    DND_MEASURE_FUNCTION();
    parsed_content = Content();
    if (!std::filesystem::exists(content_path)) {
        throw parsing_error(content_path, "does not exist");
    }
    if (!std::filesystem::directory_entry(content_path).is_directory()) {
        throw parsing_error(content_path, "is not a directory.");
    }
    if (!std::filesystem::exists(content_path / "general")) {
        parsing_error error((content_path / "general"), "does not exist.");
        error.relativiseFileName(content_path);
        throw error;
    }
    if (!std::filesystem::exists(content_path / campaign_dir_name)) {
        parsing_error error((content_path / campaign_dir_name), "does not exist.");
        error.relativiseFileName(content_path);
        throw error;
    }
    std::vector<std::filesystem::directory_entry> dirs_to_parse;
    for (const auto& source_dir : std::filesystem::directory_iterator(content_path / "general")) {
        if (!source_dir.is_directory()) {
            continue;
        }
        dirs_to_parse.push_back(source_dir);
    }
    dirs_to_parse.push_back(std::filesystem::directory_entry(content_path / campaign_dir_name));
    try {
        parseAllOfType(ParsingType::SPELL, dirs_to_parse);
        parseAllOfType(ParsingType::CLASS, dirs_to_parse);
        parseAllOfType(ParsingType::RACE, dirs_to_parse);
        parseAllOfType(ParsingType::SUBCLASS, dirs_to_parse);
        parseAllOfType(ParsingType::SUBRACE, dirs_to_parse);
        parseAllOfType(ParsingType::CHARACTER, dirs_to_parse);
    } catch (parsing_error& e) {
        e.relativiseFileName(content_path);
        throw e;
    }

    return std::move(parsed_content);
}

std::unique_ptr<dnd::ContentFileParser> dnd::ContentParser::createParser(const dnd::ParsingType parsing_type) {
    switch (parsing_type) {
        case ParsingType::CHARACTER:
            return std::make_unique<CharacterFileParser>(
                parsed_content.characters, parsed_content.character_classes, parsed_content.character_subclasses,
                parsed_content.character_races, parsed_content.character_subraces, parsed_content.spells
            );
        case ParsingType::RACE:
            return std::make_unique<CharacterRaceFileParser>(parsed_content.character_races);
        case ParsingType::SUBRACE:
            return std::make_unique<CharacterSubraceFileParser>(
                parsed_content.character_subraces, parsed_content.character_races
            );
        case ParsingType::CLASS:
            return std::make_unique<CharacterClassFileParser>(parsed_content.character_classes);
        case ParsingType::SUBCLASS:
            return std::make_unique<CharacterSubclassFileParser>(
                parsed_content.character_subclasses, parsed_content.character_classes
            );
        case ParsingType::SPELL:
            return std::make_unique<SpellsFileParser>(parsed_content.spells);
        default:
            return nullptr;
    }
}

void dnd::ContentParser::parseFileOfType(
    const dnd::ParsingType parsing_type, const std::filesystem::directory_entry& file
) {
    DND_MEASURE_SCOPE(("dnd::ContentParser::parseFileOfType ( " + subdir_names.at(parsing_type) + " )").c_str());
    std::unique_ptr<ContentFileParser> parser = createParser(parsing_type);

    if (!parser->openJSON(file)) {
        return;
    }

    try {
        parser->parse();
    } catch (const nlohmann::json::out_of_range& e) {
        const std::string stripped_what = stripJsonExceptionWhat(e.what());
        throw attribute_missing(parsing_type, file.path(), stripped_what);
    } catch (const nlohmann::json::type_error& e) {
        const std::string stripped_what = stripJsonExceptionWhat(e.what());
        throw attribute_type_error(parsing_type, file.path(), stripped_what);
    }

    if (parser->validate()) {
        std::lock_guard<std::mutex> lock(parsing_mutexes[parsing_type]);
        parser->saveResult();
    }
}

void dnd::ContentParser::parseAllOfType(
    const dnd::ParsingType parsing_type, const std::vector<std::filesystem::directory_entry>& dirs_to_parse
) {
    DND_MEASURE_SCOPE(("dnd::ContentParser::parseAllOfType ( " + subdir_names.at(parsing_type) + " )").c_str());
    std::vector<std::filesystem::directory_entry> files;
    std::vector<std::future<void>> futures;
    for (const auto& dir : dirs_to_parse) {
        std::filesystem::directory_entry type_subdir(dir.path() / subdir_names.at(parsing_type));
        if (!type_subdir.exists()) {
            continue;
        }
        if (!type_subdir.is_directory()) {
            std::cerr << "Warning: Rename " << type_subdir
                      << " so that it cannot be confused with a directory containing " << subdir_names.at(parsing_type)
                      << '\n';
            continue;
        }
        for (const auto& file : std::filesystem::directory_iterator(type_subdir)) {
            files.emplace_back(file);
        }
    }
    for (const auto& file : files) {
        futures.emplace_back(std::async(std::launch::async, &ContentParser::parseFileOfType, this, parsing_type, file));
    }
    for (auto& future : futures) {
        try {
            future.get();
        } catch (const parsing_error& e) {
            throw e;
        }
    }
}
