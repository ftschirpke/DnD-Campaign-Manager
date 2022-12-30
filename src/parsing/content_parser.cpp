#include "content_parser.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include <nlohmann/json.hpp>

#include "controllers/content.hpp"
#include "parsing/models/character_class_parser.hpp"
#include "parsing/models/content_type_parser.hpp"
#include "parsing/parsing_exceptions.hpp"
#include "parsing/parsing_types.hpp"
// #include "models/character.hpp"
// #include "models/character_class.hpp"
// #include "models/character_race.hpp"
// #include "models/spell.hpp"
// #include "parsing/models/character_class_parser.hpp"
// #include "parsing/models/character_parser.hpp"
// #include "parsing/models/character_race_parser.hpp"
// #include "parsing/models/spell_parser.hpp"

// std::string stripJsonExceptionWhat(const std::string& original_what) {
//     std::string what = original_what;
//     if (int i = what.find("]"); i != std::string::npos) {
//         what.erase(0, i + 2);
//     }
//     return '(' + what + ')';
// }

// const std::unique_ptr<const nlohmann::json> dnd::ContentParser::openJSON(const std::filesystem::directory_entry&
// file) {
//     if (!file.is_regular_file()) {
//         std::cerr << "Warning: " << file.path() << " is not a regular file.\n";
//         return nullptr;
//     }
//     const std::string path = file.path().c_str();
//     if (path.compare(path.length() - 5, path.length(), ".json") != 0) {
//         std::cerr << "Warning: \"" << path << "\" is not a \".json\" file.\n";
//         return nullptr;
//     }
//     std::ifstream json_file(path);
//     nlohmann::json j;
//     try {
//         json_file >> j;
//     } catch (const nlohmann::json::parse_error& e) {
//         std::cerr << "Warning: ParseError occured while parsing \"" << path << "\":\n" << e.what() << '\n';
//         return nullptr;
//     }
//     std::unique_ptr<const nlohmann::json> json_ptr = std::make_unique<nlohmann::json>(std::move(j));
//     return json_ptr;
// }

// void dnd::ContentParser::parseSpells(const std::filesystem::path& directory) {
//     if (!std::filesystem::directory_entry(directory).is_directory()) {
//         std::cerr << "Warning: Rename " << directory
//                   << " so that it cannot be confused with a directory containing spells.\n";
//         return;
//     }
//     for (const auto& entry : std::filesystem::directory_iterator(directory)) {
//         const auto spells_json = openJSON(entry);
//         if (spells_json == nullptr) {
//             continue;
//         }
//         if (!spells_json->is_object()) {
//             const std::string filename = entry.path().c_str();
//             throw parsing_error("Spell file \"" + filename + "\" is not formatted as an object/map.");
//         }
//         for (const auto& [spell_name, spell_info] : spells_json->items()) {
//             try {
//                 std::shared_ptr<const Spell> spell = SpellParser::createSpell(spell_name, spell_info);
//                 if (content.spells.find(spell_name) != content.spells.end()) {
//                     std::cerr << "Warning: Duplicate of spell \"" << spell_name << "\" found in " << entry.path()
//                               << ".\n";
//                 } else {
//                     content.spells.emplace(spell_name, std::move(spell));
//                 }
//             } catch (const nlohmann::json::out_of_range& e) {
//                 const std::string json_what = stripJsonExceptionWhat(e.what());
//                 throw parsing_error("Spell \"" + spell_name + "\" is missing a required value. " + json_what);
//             } catch (const nlohmann::json::type_error& e) {
//                 const std::string json_what = stripJsonExceptionWhat(e.what());
//                 throw parsing_error("Spell \"" + spell_name + "\" has attribute of wrong type. " + json_what);
//             } catch (const std::invalid_argument& e) {
//                 throw parsing_error("Spell \"" + spell_name + "\": " + e.what());
//             }
//         }
//     }
// }

// void dnd::ContentParser::parseCharacters(const std::filesystem::path& directory) {
//     if (!std::filesystem::directory_entry(directory).is_directory()) {
//         std::cerr << "Warning: Rename " << directory
//                   << " so that it cannot be confused with a directory containing characters.\n";
//         return;
//     }
//     for (const auto& entry : std::filesystem::directory_iterator(directory)) {
//         const auto character_json = openJSON(entry);
//         if (character_json == nullptr) {
//             continue;
//         }
//         const std::string filename = entry.path().lexically_relative(content_path).c_str();
//         if (!character_json->is_object()) {
//             throw parsing_error("Character file \"" + filename + "\" is not formatted as an object/map.");
//         }
//         try {
//             std::shared_ptr<const Character> character = CharacterParser::createCharacter(*character_json, content);
//             if (content.characters.find(character->name) != content.characters.end()) {
//                 std::cerr << "Warning: Duplicate of character \"" << character->name << "\" found in " <<
//                 entry.path()
//                           << ".\n";
//             } else {
//                 content.characters.emplace(character->name, std::move(character));
//             }
//         } catch (const nlohmann::json::out_of_range& e) {
//             const std::string json_what = stripJsonExceptionWhat(e.what());
//             throw parsing_error("Character in file \"" + filename + "\" is missing a required value. " + json_what);
//         } catch (const nlohmann::json::type_error& e) {
//             const std::string json_what = stripJsonExceptionWhat(e.what());
//             throw parsing_error("Character in file \"" + filename + "\" has attribute of wrong type. " + json_what);
//         } catch (const std::invalid_argument& e) {
//             throw parsing_error(e.what());
//         }
//     }
// }

// void dnd::ContentParser::parseCharacterClasses(const std::filesystem::path& directory) {
//     if (!std::filesystem::directory_entry(directory).is_directory()) {
//         std::cerr << "Warning: Rename " << directory
//                   << " so that it cannot be confused with a directory containing classes.\n";
//         return;
//     }
//     for (const auto& entry : std::filesystem::directory_iterator(directory)) {
//         const auto character_class_json = openJSON(entry);
//         if (character_class_json == nullptr) {
//             continue;
//         }
//         const std::string filename = entry.path().lexically_relative(content_path).c_str();
//         if (!character_class_json->is_object()) {
//             throw parsing_error("Class file \"" + filename + "\" is not formatted as an object/map.");
//         }
//         try {
//             std::shared_ptr<const CharacterClass> character_class =
//                 CharacterClassParser::createCharacterClass(*character_class_json);
//             if (content.character_classes.find(character_class->name) != content.character_classes.end()) {
//                 std::cerr << "Warning: Duplicate of character class \"" << character_class->name << "\" found in "
//                           << entry.path() << ".\n";
//             } else {
//                 content.character_classes.emplace(character_class->name, std::move(character_class));
//             }
//         } catch (const nlohmann::json::out_of_range& e) {
//             const std::string json_what = stripJsonExceptionWhat(e.what());
//             throw parsing_error(
//                 "Character Class in file \"" + filename + "\" is missing a required value. " + json_what
//             );
//         } catch (const nlohmann::json::type_error& e) {
//             const std::string json_what = stripJsonExceptionWhat(e.what());
//             throw parsing_error(
//                 "Character Class in file \"" + filename + "\" has attribute of wrong type. " + json_what
//             );
//         } catch (const std::invalid_argument& e) {
//             throw parsing_error("Character Class in file \"" + filename + "\": " + e.what());
//         }
//     }
// }

// void dnd::ContentParser::parseCharacterSubclasses(const std::filesystem::path& directory) {
//     if (!std::filesystem::directory_entry(directory).is_directory()) {
//         std::cerr << "Warning: Rename " << directory
//                   << " so that it cannot be confused with a directory containing subclasses.\n";
//         return;
//     }
//     for (const auto& entry : std::filesystem::directory_iterator(directory)) {
//         const auto character_subclass_json = openJSON(entry);
//         if (character_subclass_json == nullptr) {
//             continue;
//         }
//         const std::string filename = entry.path().lexically_relative(content_path).c_str();
//         if (!character_subclass_json->is_object()) {
//             throw parsing_error("Subclass file \"" + filename + "\" is not formatted as an object/map.");
//         }
//         try {
//             std::shared_ptr<const CharacterSubclass> character_subclass =
//                 CharacterClassParser::createCharacterSubclass(*character_subclass_json);
//             if (content.character_subclasses.find(character_subclass->name)
//                 != content.character_subclasses.end()) {
//                 std::cerr << "Warning: Duplicate of character subclass \"" << character_subclass->name << "\" found
//                 in "
//                           << entry.path() << ".\n";
//             } else {
//                 content.character_subclasses.emplace(character_subclass->name, std::move(character_subclass));
//             }
//         } catch (const nlohmann::json::out_of_range& e) {
//             const std::string json_what = stripJsonExceptionWhat(e.what());
//             throw parsing_error(
//                 "Character Subclass in file \"" + filename + "\" is missing a required value. " + json_what
//             );
//         } catch (const nlohmann::json::type_error& e) {
//             const std::string json_what = stripJsonExceptionWhat(e.what());
//             throw parsing_error(
//                 "Character Subclass in file \"" + filename + "\" has attribute of wrong type. " + json_what
//             );
//         } catch (const std::invalid_argument& e) {
//             throw parsing_error("Character Subclass in file \"" + filename + "\": " + e.what());
//         }
//     }
// }

// void dnd::ContentParser::parseCharacterRaces(const std::filesystem::path& directory) {
//     if (!std::filesystem::directory_entry(directory).is_directory()) {
//         std::cerr << "Warning: Rename " << directory
//                   << " so that it cannot be confused with a directory containing races.\n";
//         return;
//     }
//     for (const auto& entry : std::filesystem::directory_iterator(directory)) {
//         const auto character_race_json = openJSON(entry);
//         if (character_race_json == nullptr) {
//             continue;
//         }
//         const std::string filename = entry.path().lexically_relative(content_path).c_str();
//         if (!character_race_json->is_object()) {
//             throw parsing_error("Race file \"" + filename + "\" is not formatted as an object/map.");
//         }
//         try {
//             std::shared_ptr<const CharacterRace> character_race =
//                 CharacterRaceParser::createCharacterRace(*character_race_json);
//             if (content.character_races.find(character_race->name) != content.character_races.end()) {
//                 std::cerr << "Warning: Duplicate of character race \"" << character_race->name << "\" found in "
//                           << entry.path() << ".\n";
//             } else {
//                 content.character_races.emplace(character_race->name, std::move(character_race));
//             }
//         } catch (const nlohmann::json::out_of_range& e) {
//             const std::string json_what = stripJsonExceptionWhat(e.what());
//             throw parsing_error(
//                 "Character Race in file \"" + filename + "\" is missing a required value. " + json_what
//             );
//         } catch (const nlohmann::json::type_error& e) {
//             const std::string json_what = stripJsonExceptionWhat(e.what());
//             throw parsing_error(
//                 "Character Race in file \"" + filename + "\" has attribute of wrong type. " + json_what
//             );
//         } catch (const std::invalid_argument& e) {
//             throw parsing_error("Character Race in file \"" + filename + "\": " + e.what());
//         }
//     }
// }

// void dnd::ContentParser::parseCharacterSubraces(const std::filesystem::path& directory) {
//     if (!std::filesystem::directory_entry(directory).is_directory()) {
//         std::cerr << "Warning: Rename " << directory
//                   << " so that it cannot be confused with a directory containing subraces.\n";
//         return;
//     }
//     for (const auto& entry : std::filesystem::directory_iterator(directory)) {
//         const auto character_subrace_json = openJSON(entry);
//         if (character_subrace_json == nullptr) {
//             continue;
//         }
//         const std::string filename = entry.path().lexically_relative(content_path).c_str();
//         if (!character_subrace_json->is_object()) {
//             throw parsing_error("Subrace file \"" + filename + "\" is not formatted as an object/map.");
//         }
//         try {
//             std::shared_ptr<const CharacterSubrace> character_subrace =
//                 CharacterRaceParser::createCharacterSubrace(*character_subrace_json);
//             if (content.character_subraces.find(character_subrace->name) != content.character_subraces.end()) {
//                 std::cerr << "Warning: Duplicate of character subrace \"" << character_subrace->name << "\" found in
//                 "
//                           << entry.path() << ".\n";
//             } else {
//                 content.character_subraces.emplace(character_subrace->name, std::move(character_subrace));
//             }
//         } catch (const nlohmann::json::out_of_range& e) {
//             const std::string json_what = stripJsonExceptionWhat(e.what());
//             throw parsing_error(
//                 "Character Subrace in file \"" + filename + "\" is missing a required value. " + json_what
//             );
//         } catch (const nlohmann::json::type_error& e) {
//             const std::string json_what = stripJsonExceptionWhat(e.what());
//             throw parsing_error(
//                 "Character Subrace in file \"" + filename + "\" has attribute of wrong type. " + json_what
//             );
//         } catch (const std::invalid_argument& e) {
//             throw parsing_error("Character Subrace in file \"" + filename + "\": " + e.what());
//         }
//     }
// }

// void dnd::ContentParser::validateCharacterSubclasses() const {
//     bool all_valid = true;
//     for (const auto& [subclass_name, subclass_ptr] : content.character_subclasses) {
//         if (content.character_classes.find(subclass_ptr->class_name) == content.character_classes.cend()) {
//             std::cerr << "Error: Subclass \"" << subclass_name << "\" is invalid. The class \""
//                       << subclass_ptr->class_name << "\" does not exist.\n";
//             all_valid = false;
//         }
//     }
//     if (!all_valid) {
//         throw parsing_error("Invalid character subclasses");
//     }
// }

// void dnd::ContentParser::validateCharacterSubraces() const {
//     bool all_valid = true;
//     for (const auto& [subrace_name, subrace_ptr] : content.character_subraces) {
//         try {
//             std::shared_ptr<const CharacterRace> race_ptr = content.character_races.at(subrace_ptr->race_name);
//             if (!race_ptr->has_subraces) {
//                 std::cerr << "Error: Subrace \"" << subrace_name << "\" is invalid. \"" << race_ptr->name
//                           << "\" does not have subraces.\n";
//                 all_valid = false;
//             }
//         } catch (const std::out_of_range& e) {
//             std::cerr << "Error: Subrace \"" << subrace_name << "\" is invalid. The race \"" <<
//             subrace_ptr->race_name
//                       << "\" does not exist.\n";
//             all_valid = false;
//         }
//     }
//     if (!all_valid) {
//         throw parsing_error("Invalid character subraces");
//     }
// }

// void dnd::ContentParser::parseAll() {
//     if (!std::filesystem::exists(content_path)) {
//         std::stringstream sstr;
//         sstr << content_path << " does not exist.";
//         throw std::invalid_argument(sstr.str());
//     }
//     if (!std::filesystem::directory_entry(content_path).is_directory()) {
//         std::stringstream sstr;
//         sstr << content_path << " is not a directory.";
//         throw std::invalid_argument(sstr.str());
//     }
//     if (!std::filesystem::exists(content_path / "general")) {
//         throw std::invalid_argument("No subdirectory 'general' exists within the content directory.");
//     }
//     if (!std::filesystem::exists(content_path / campaign_dir_name)) {
//         std::stringstream sstr;
//         sstr << "No subdirectory '" << campaign_dir_name << "' exists within the content directory.";
//         throw std::invalid_argument(sstr.str());
//     }
//     std::vector<std::filesystem::directory_entry> dirs_to_parse;
//     for (const auto& source_dir : std::filesystem::directory_iterator(content_path / "general")) {
//         if (!source_dir.is_directory()) {
//             continue;
//         }
//         dirs_to_parse.push_back(source_dir);
//     }
//     dirs_to_parse.push_back(std::filesystem::directory_entry(content_path / campaign_dir_name));

//     for (const auto& dir : dirs_to_parse) {
//         // TODO: keep or delete these messages giving feeling of progress?
//         // std::cout << "Parsing " << dir.path().lexically_relative(content_path) << " directory...\n";
//         if (std::filesystem::exists(dir.path() / "spells")) {
//             parseSpells(dir.path() / "spells");
//         }
//         if (std::filesystem::exists(dir.path() / "classes")) {
//             parseCharacterClasses(dir.path() / "classes");
//         }
//         if (std::filesystem::exists(dir.path() / "subclasses")) {
//             parseCharacterSubclasses(dir.path() / "subclasses");
//         }
//         if (std::filesystem::exists(dir.path() / "races")) {
//             parseCharacterRaces(dir.path() / "races");
//         }
//         if (std::filesystem::exists(dir.path() / "subraces")) {
//             parseCharacterSubraces(dir.path() / "subraces");
//         }
//     }
//     validateCharacterSubclasses();
//     validateCharacterSubraces();

//     // TODO: keep or delete these messages giving feeling of progress?
//     // std::cout << "Parsing characters...\n";
//     for (const auto& dir : dirs_to_parse) {
//         if (std::filesystem::exists(dir.path() / "characters")) {
//             parseCharacters(dir.path() / "characters");
//         }
//     }
// }

dnd::Content dnd::ContentParser::parse(
    const std::filesystem::path& content_path, const std::string& campaign_dir_name
) {
    if (!std::filesystem::exists(content_path)) {
        std::stringstream sstr;
        sstr << content_path << " does not exist.";
        throw std::invalid_argument(sstr.str());
    }
    if (!std::filesystem::directory_entry(content_path).is_directory()) {
        std::stringstream sstr;
        sstr << content_path << " is not a directory.";
        throw std::invalid_argument(sstr.str());
    }
    if (!std::filesystem::exists(content_path / "general")) {
        throw std::invalid_argument("No subdirectory 'general' exists within the content directory.");
    }
    if (!std::filesystem::exists(content_path / campaign_dir_name)) {
        std::stringstream sstr;
        sstr << "No subdirectory '" << campaign_dir_name << "' exists within the content directory.";
        throw std::invalid_argument(sstr.str());
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
    std::unique_ptr<ContentTypeParser> parser;
    switch (parsing_type) {
        case ParsingType::CLASSES:
            parser = std::make_unique<CharacterClassParser>(parsed_character_classes);
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
                throw attribute_missing(file.path(), stripped_what, parsing_type);
            } catch (const nlohmann::json::type_error& e) {
                const std::string stripped_what = stripJsonExceptionWhat(e.what());
                throw attribute_type_error(file.path(), stripped_what, parsing_type);
            }

            if (parser->validate()) {
                parser->saveResult();
            }
            parser->reset();
        }
    }
}
