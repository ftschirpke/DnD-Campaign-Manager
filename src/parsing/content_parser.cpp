#include "content_parser.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

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
        parseType(ParsingType::SPELLS, dirs_to_parse);
        parseType(ParsingType::RACES, dirs_to_parse);
        parseType(ParsingType::CLASSES, dirs_to_parse);
        parseType(ParsingType::SUBRACES, dirs_to_parse);
        parseType(ParsingType::SUBCLASSES, dirs_to_parse);
        parseType(ParsingType::CHARACTERS, dirs_to_parse);
    } catch (parsing_error& e) {
        e.relativiseFileName(content_path);
        throw e;
    }

    // TODO: change Content constructor
    Content content;
    content.spells = parsed_spells;
    content.characters = parsed_characters;
    content.character_classes = parsed_character_classes;
    content.character_subclasses = parsed_character_subclasses;
    content.character_races = parsed_character_races;
    content.character_subraces = parsed_character_subraces;
    return content;
}

void dnd::ContentParser::parseType(
    const dnd::ParsingType parsing_type, const std::vector<std::filesystem::directory_entry>& dirs_to_parse
) {
    std::unique_ptr<ContentFileParser> parser;
    switch (parsing_type) {
        case ParsingType::CHARACTERS:
            parser = std::make_unique<CharacterFileParser>(parsed_characters);
            break;
        case ParsingType::RACES:
            parser = std::make_unique<CharacterRaceFileParser>(parsed_character_races);
            break;
        case ParsingType::SUBRACES:
            parser = std::make_unique<CharacterSubraceFileParser>(parsed_character_subraces, parsed_character_races);
            break;
        case ParsingType::CLASSES:
            parser = std::make_unique<CharacterClassFileParser>(parsed_character_classes);
            break;
        case ParsingType::SUBCLASSES:
            parser =
                std::make_unique<CharacterSubclassFileParser>(parsed_character_subclasses, parsed_character_classes);
            break;
        case ParsingType::SPELLS:
            parser = std::make_unique<SpellsFileParser>(parsed_spells);
            break;
        default:
            return;
    }
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
            if (!parser->openJSON(file)) {
                continue;
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
                parser->saveResult();
            }
            parser->reset();
        }
    }
}
