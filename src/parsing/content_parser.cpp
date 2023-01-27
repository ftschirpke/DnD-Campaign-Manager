#include "dnd_config.hpp"

#include "content_parser.hpp"

#include <filesystem>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include "controllers/content.hpp"
#include "parsing/content_file_parser.hpp"
#include "parsing/effect_holder_groups_file_parser.hpp"
#include "parsing/models/character_class_file_parser.hpp"
#include "parsing/models/character_file_parser.hpp"
#include "parsing/models/character_race_file_parser.hpp"
#include "parsing/models/character_subclass_file_parser.hpp"
#include "parsing/models/character_subrace_file_parser.hpp"
#include "parsing/models/spells_file_parser.hpp"
#include "parsing/parsing_exceptions.hpp"
#include "parsing/parsing_types.hpp"
#include "parsing/string_groups_file_parser.hpp"

void dnd::ContentParser::reset() noexcept {
    parsed_content = Content();
    dirs_to_parse = std::vector<std::filesystem::directory_entry>();
}

dnd::Content dnd::ContentParser::parse(
    const std::filesystem::path& content_path, const std::string& campaign_dir_name
) {
    DND_MEASURE_FUNCTION();
    reset();
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

    for (const auto& source_dir : std::filesystem::directory_iterator(content_path / "general")) {
        if (!source_dir.is_directory()) {
            continue;
        }
        dirs_to_parse.push_back(source_dir);
    }
    dirs_to_parse.push_back(std::filesystem::directory_entry(content_path / campaign_dir_name));
    try {
        parseAllOfType(ParsingType::GROUP);
        parseAllOfType(ParsingType::SPELL);
        parseAllOfType(ParsingType::CLASS);
        parseAllOfType(ParsingType::RACE);
        parseAllOfType(ParsingType::SUBCLASS);
        parseAllOfType(ParsingType::SUBRACE);
        parseAllOfType(ParsingType::CHARACTER);
    } catch (parsing_error& e) {
        e.relativiseFileName(content_path);
        throw e;
    }

    return std::move(parsed_content);
}

std::unique_ptr<dnd::ContentFileParser> dnd::ContentParser::createSingleFileParserForType(
    const dnd::ParsingType parsing_type
) {
    switch (parsing_type) {
        case ParsingType::GROUP:
            return std::make_unique<StringGroupsFileParser>(parsed_content.groups);
        default:
            return createGeneralParserForType(parsing_type);
    }
}

std::unique_ptr<dnd::ContentFileParser> dnd::ContentParser::createMultiFileParserForType(
    const dnd::ParsingType parsing_type
) {
    switch (parsing_type) {
        case ParsingType::GROUP:
            return std::make_unique<EffectHolderGroupsFileParser>(parsed_content.groups);
        default:
            return createGeneralParserForType(parsing_type);
    }
}

std::unique_ptr<dnd::ContentFileParser> dnd::ContentParser::createGeneralParserForType(
    const dnd::ParsingType parsing_type
) {
    switch (parsing_type) {
        case ParsingType::CHARACTER:
            return std::make_unique<CharacterFileParser>(
                parsed_content.characters, parsed_content.groups, parsed_content.character_classes,
                parsed_content.character_subclasses, parsed_content.character_races, parsed_content.character_subraces,
                parsed_content.spells
            );
        case ParsingType::RACE:
            return std::make_unique<CharacterRaceFileParser>(parsed_content.character_races, parsed_content.groups);
        case ParsingType::SUBRACE:
            return std::make_unique<CharacterSubraceFileParser>(
                parsed_content.character_subraces, parsed_content.groups, parsed_content.character_races
            );
        case ParsingType::CLASS:
            return std::make_unique<CharacterClassFileParser>(
                parsed_content.character_classes, parsed_content.groups, parsed_content.spells
            );
        case ParsingType::SUBCLASS:
            return std::make_unique<CharacterSubclassFileParser>(
                parsed_content.character_subclasses, parsed_content.groups, parsed_content.character_classes,
                parsed_content.spells
            );
        case ParsingType::SPELL:
            return std::make_unique<SpellsFileParser>(parsed_content.spells, parsed_content.groups);
        default:
            throw std::logic_error(
                "No general parser for type \"" + parsing_type_names.at(parsing_type) + "\" implemented."
            );
    }
}

void dnd::ContentParser::parseFileOfType(
    const std::filesystem::directory_entry& file, const ParsingType parsing_type, bool multi_file
) {
    DND_MEASURE_SCOPE(("dnd::ContentParser::parseFileOfType ( " + parsing_type_names.at(parsing_type) + ", "
                       + (multi_file ? "multi-file" : "single-file") + " )")
                          .c_str());

    std::unique_ptr<ContentFileParser> parser;
    if (multi_file) {
        parser = createMultiFileParserForType(parsing_type);
    } else {
        parser = createSingleFileParserForType(parsing_type);
    }

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

void dnd::ContentParser::parseAllOfType(const dnd::ParsingType parsing_type) {
    switch (parsing_type) {
        case ParsingType::GROUP:
            // groups need single-file and multi-file parsing
            parseAllOfSingleFileType(parsing_type);
            [[fallthrough]];
        default:
            parseAllOfMultiFileType(parsing_type);
            break;
    }
}

void dnd::ContentParser::parseAllOfSingleFileType(const ParsingType parsing_type) {
    DND_MEASURE_SCOPE(
        ("dnd::ContentParser::parseAllOfSingleFileType ( " + parsing_type_names.at(parsing_type) + " )").c_str()
    );
    std::vector<std::filesystem::directory_entry> files_to_parse;
    std::vector<std::future<void>> futures;
    for (const auto& dir : dirs_to_parse) {
        std::filesystem::directory_entry type_file;
        try {
            type_file = std::filesystem::directory_entry(dir.path() / (file_names.at(parsing_type) + ".json"));
        } catch (const std::out_of_range& e) {
            throw std::logic_error(
                "Cannot parse type \"" + parsing_type_names.at(parsing_type) + "\" as single file type."
            );
        }
        if (!type_file.exists()) {
            continue;
        }
        if (!type_file.is_regular_file()) {
            std::cerr << "Warning: Rename " << type_file << " so that it cannot be confused with a file containing "
                      << file_names.at(parsing_type) << '\n';
            continue;
        }
        files_to_parse.emplace_back(type_file);
    }

    for (const auto& file : files_to_parse) {
        futures.emplace_back(
            std::async(std::launch::async, &ContentParser::parseFileOfType, this, file, parsing_type, false)
        );
    }
    for (auto& future : futures) {
        try {
            future.get();
        } catch (const parsing_error& e) {
            throw e;
        }
    }
}

void dnd::ContentParser::parseAllOfMultiFileType(const ParsingType parsing_type) {
    DND_MEASURE_SCOPE(
        ("dnd::ContentParser::parseAllOfMultiFileType ( " + parsing_type_names.at(parsing_type) + " )").c_str()
    );
    std::vector<std::filesystem::directory_entry> files_to_parse;
    std::vector<std::future<void>> futures;
    for (const auto& dir : dirs_to_parse) {
        std::filesystem::directory_entry type_subdir;
        try {
            type_subdir = std::filesystem::directory_entry(dir.path() / subdir_names.at(parsing_type));
        } catch (const std::out_of_range& e) {
            throw std::logic_error(
                "Cannot parse type \"" + parsing_type_names.at(parsing_type) + "\" as multi file type."
            );
        }

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
            files_to_parse.emplace_back(file);
        }
    }

    for (const auto& file : files_to_parse) {
        futures.emplace_back(
            std::async(std::launch::async, &ContentParser::parseFileOfType, this, file, parsing_type, true)
        );
    }
    for (auto& future : futures) {
        try {
            future.get();
        } catch (const parsing_error& e) {
            throw e;
        }
    }
}
