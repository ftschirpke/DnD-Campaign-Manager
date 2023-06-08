#include <dnd_config.hpp>

#include "content_parser.hpp"

#include <array>
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

#include <core/controllers/content_holder.hpp>
#include <core/parsing/content_file_parser.hpp>
#include <core/parsing/controllers/effect_holder_groups_file_parser.hpp>
#include <core/parsing/controllers/string_groups_file_parser.hpp>
#include <core/parsing/models/character_class_file_parser.hpp>
#include <core/parsing/models/character_file_parser.hpp>
#include <core/parsing/models/character_race_file_parser.hpp>
#include <core/parsing/models/character_subclass_file_parser.hpp>
#include <core/parsing/models/character_subrace_file_parser.hpp>
#include <core/parsing/models/items_file_parser.hpp>
#include <core/parsing/models/spells_file_parser.hpp>
#include <core/parsing/parsing_exceptions.hpp>
#include <core/parsing/parsing_types.hpp>

constexpr std::array<std::pair<dnd::ParsingType, const char*>, 1> dnd::ContentParser::file_names = {
    std::pair(ParsingType::GROUP, "groups"),
};

constexpr std::array<std::pair<dnd::ParsingType, const char*>, 8> dnd::ContentParser::subdir_names = {
    std::pair(ParsingType::GROUP, "groups"),  std::pair(ParsingType::CHARACTER, "characters"),
    std::pair(ParsingType::CLASS, "classes"), std::pair(ParsingType::SUBCLASS, "subclasses"),
    std::pair(ParsingType::RACE, "races"),    std::pair(ParsingType::SUBRACE, "subraces"),
    std::pair(ParsingType::ITEM, "items"),    std::pair(ParsingType::SPELL, "spells"),
};

void dnd::ContentParser::reset() noexcept {
    parsed_content = ContentHolder();
    dirs_to_parse = std::vector<std::filesystem::directory_entry>();
}

dnd::ContentHolder dnd::ContentParser::parse(
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
        error.relativize_file_name(content_path);
        throw error;
    }
    if (!std::filesystem::exists(content_path / campaign_dir_name)) {
        parsing_error error((content_path / campaign_dir_name), "does not exist.");
        error.relativize_file_name(content_path);
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
        parseAllOfType(ParsingType::ITEM);
        parseAllOfType(ParsingType::SPELL);
        parseAllOfType(ParsingType::CLASS);
        parseAllOfType(ParsingType::RACE);
        parseAllOfType(ParsingType::SUBCLASS);
        parseAllOfType(ParsingType::SUBRACE);
        parseAllOfType(ParsingType::CHARACTER);
    } catch (parsing_error& e) {
        e.relativize_file_name(content_path);
        throw e;
    }

    return std::move(parsed_content);
}

std::unique_ptr<dnd::ContentFileParser> dnd::ContentParser::createSingleFileParser(
    const std::filesystem::path filepath, const dnd::ParsingType parsing_type
) {
    switch (parsing_type) {
        case ParsingType::GROUP:
            return std::make_unique<StringGroupsFileParser>(filepath, parsed_content.groups);
        default:
            throw std::logic_error(
                "No single-file parser for content type \"" + std::string(parsing_type_name(parsing_type))
                + "\" exists."
            );
    }
}

std::unique_ptr<dnd::ContentFileParser> dnd::ContentParser::createMultiFileParser(
    const std::filesystem::path filepath, const dnd::ParsingType parsing_type
) {
    switch (parsing_type) {
        case ParsingType::CHARACTER:
            return std::make_unique<CharacterFileParser>(
                filepath, parsed_content.characters, parsed_content.groups, parsed_content.character_classes,
                parsed_content.character_subclasses, parsed_content.character_races, parsed_content.character_subraces,
                parsed_content.spells
            );
        case ParsingType::RACE:
            return std::make_unique<CharacterRaceFileParser>(
                filepath, parsed_content.character_races, parsed_content.groups
            );
        case ParsingType::SUBRACE:
            return std::make_unique<CharacterSubraceFileParser>(
                filepath, parsed_content.character_subraces, parsed_content.groups, parsed_content.character_races
            );
        case ParsingType::CLASS:
            return std::make_unique<CharacterClassFileParser>(
                filepath, parsed_content.character_classes, parsed_content.groups, parsed_content.spells
            );
        case ParsingType::SUBCLASS:
            return std::make_unique<CharacterSubclassFileParser>(
                filepath, parsed_content.character_subclasses, parsed_content.groups, parsed_content.character_classes,
                parsed_content.spells
            );
        case ParsingType::ITEM:
            return std::make_unique<ItemsFileParser>(filepath, parsed_content.items, parsed_content.groups);
        case ParsingType::SPELL:
            return std::make_unique<SpellsFileParser>(filepath, parsed_content.spells, parsed_content.groups);
        case ParsingType::GROUP:
            return std::make_unique<EffectHolderGroupsFileParser>(filepath, parsed_content.groups);
        default:
            throw std::logic_error(
                "No multi-file parser for content type \"" + std::string(parsing_type_name(parsing_type)) + "\" exists."
            );
    }
}

void dnd::ContentParser::parseFile(std::unique_ptr<ContentFileParser> parser) {
    DND_MEASURE_SCOPE(("dnd::ContentParser::parseFile ( " + std::string(parsing_type_name(parser->get_type())) + ", "
                       + parser->filepath.filename().string() + " )")
                          .c_str());

    if (!parser->open_json()) {
        return;
    }

    try {
        parser->parse();
    } catch (const nlohmann::json::out_of_range& e) {
        const std::string stripped_what = strip_json_exception_what(e.what());
        throw attribute_missing(parser->get_type(), parser->filepath, stripped_what);
    } catch (const nlohmann::json::type_error& e) {
        const std::string stripped_what = strip_json_exception_what(e.what());
        throw attribute_type_error(parser->get_type(), parser->filepath, stripped_what);
    }

    if (parser->validate()) {
        std::lock_guard<std::mutex> lock(parsing_mutexes[parser->get_type()]);
        parser->save_result();
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
    DND_MEASURE_SCOPE(("dnd::ContentParser::parseAllOfSingleFileType ( " + std::string(parsing_type_name(parsing_type))
                       + " )")
                          .c_str());
    std::vector<std::unique_ptr<ContentFileParser>> to_parse;
    std::vector<std::future<void>> futures;
    for (const auto& dir : dirs_to_parse) {
        auto type_file_it = std::find_if(
            file_names.cbegin(), file_names.cend(),
            [parsing_type](const std::pair<ParsingType, const char*>& p) { return p.first == parsing_type; }
        );
        if (type_file_it == file_names.end()) {
            throw std::logic_error(
                "Cannot parse type \"" + std::string(parsing_type_name(parsing_type)) + "\" as single-file type."
            );
        }
        const std::string type_file_name = type_file_it->second;
        std::filesystem::directory_entry type_file = std::filesystem::directory_entry(
            dir.path() / (type_file_name + ".json")
        );
        if (!type_file.exists()) {
            continue;
        }
        if (!type_file.is_regular_file()) {
            std::cerr << "Warning: Rename " << type_file.path().string()
                      << " so that it cannot be confused with a file containing " << type_file_name << '\n';
            continue;
        }
        to_parse.emplace_back(createSingleFileParser(type_file.path(), parsing_type));
    }

    for (auto& parser : to_parse) {
        futures.emplace_back(std::async(std::launch::async, &ContentParser::parseFile, this, std::move(parser)));
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
        ("dnd::ContentParser::parseAllOfMultiFileType ( " + std::string(parsing_type_name(parsing_type)) + " )").c_str()
    );
    std::vector<std::unique_ptr<ContentFileParser>> to_parse;
    std::vector<std::future<void>> futures;
    for (const auto& dir : dirs_to_parse) {
        auto type_dir_it = std::find_if(
            subdir_names.cbegin(), subdir_names.cend(),
            [parsing_type](const std::pair<ParsingType, const char*>& p) { return p.first == parsing_type; }
        );
        if (type_dir_it == subdir_names.end()) {
            throw std::logic_error(
                "Cannot parse type \"" + std::string(parsing_type_name(parsing_type)) + "\" as multi-file type."
            );
        }
        const std::string type_dir_name = type_dir_it->second;
        std::filesystem::directory_entry type_subdir = std::filesystem::directory_entry(dir.path() / type_dir_name);

        if (!type_subdir.exists()) {
            continue;
        }
        if (!type_subdir.is_directory()) {
            std::cerr << "Warning: Rename " << type_subdir.path().string()
                      << " so that it cannot be confused with a directory containing " << type_dir_name << '\n';
            continue;
        }
        for (const auto& file : std::filesystem::directory_iterator(type_subdir)) {
            if (!file.is_regular_file()) {
                std::cerr << "Warning: " << file.path() << " is not a regular file.\n";
                continue;
            }
            to_parse.emplace_back(createMultiFileParser(file.path(), parsing_type));
        }
    }

    for (auto& parser : to_parse) {
        futures.emplace_back(std::async(std::launch::async, &ContentParser::parseFile, this, std::move(parser)));
    }
    for (auto& future : futures) {
        try {
            future.get();
        } catch (const parsing_error& e) {
            throw e;
        }
    }
}
