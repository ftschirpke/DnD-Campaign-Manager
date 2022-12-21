#include "content_parser.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include <nlohmann/json.hpp>

#include "controllers/content_controller.hpp"
#include "models/character_class.hpp"
#include "models/character_race.hpp"
#include "models/spell.hpp"
#include "parsing/models/character_class_parser.hpp"
#include "parsing/models/character_race_parser.hpp"
#include "parsing/models/spell_parser.hpp"

const std::unique_ptr<const nlohmann::json> dnd::ContentParser::openJSON(const std::filesystem::directory_entry& file) {
    if (!file.is_regular_file()) {
        std::cerr << "Warning: " << file.path() << " is not a regular file.\n";
        return nullptr;
    }
    const std::string path = file.path().c_str();
    if (path.compare(path.length() - 5, path.length(), ".json") != 0) {
        std::cerr << "Warning: \"" << path << "\" is not a \".json\" file.\n";
        return nullptr;
    }
    std::ifstream json_file(path);
    nlohmann::json j;
    try {
        json_file >> j;
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Warning: ParseError occured while parsing \"" << path << "\":\n" << e.what() << '\n';
        return nullptr;
    }
    std::unique_ptr<const nlohmann::json> json_ptr = std::make_unique<nlohmann::json>(std::move(j));
    return json_ptr;
}

void dnd::ContentParser::parseSpells(const std::filesystem::path& directory) {
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        const auto spells_json = openJSON(entry);
        if (spells_json == nullptr) {
            continue;
        }
        if (!spells_json->is_object()) {
            const std::string filename = entry.path().c_str();
            throw parsing_error("Spell file \"" + filename + "\" is not formatted as an object/map.");
        }
        for (const auto& [spell_name, spell_info] : spells_json->items()) {
            try {
                std::shared_ptr<const Spell> spell = SpellParser::createSpell(spell_name, spell_info);
                if (controller.spells.find(spell_name) != controller.spells.end()) {
                    std::cerr << "Warning: Duplicate of spell \"" << spell_name << "\" found in " << entry.path()
                              << ".\n";
                } else {
                    controller.spells.emplace(spell_name, std::move(spell));
                }
            } catch (const nlohmann::json::out_of_range& e) {
                throw parsing_error("Spell \"" + spell_name + "\" is missing an attribute.");
            } catch (const std::invalid_argument& e) {
                throw parsing_error("Spell \"" + spell_name + "\": " + e.what());
            }
        }
    }
}

void dnd::ContentParser::parseCharacterClasses(const std::filesystem::path& directory) {
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        const auto character_class_json = openJSON(entry);
        if (character_class_json == nullptr) {
            continue;
        }
        const std::string filename = entry.path().c_str();
        if (!character_class_json->is_object()) {
            throw parsing_error("Class file \"" + filename + "\" is not formatted as an object/map.");
        }
        try {
            std::shared_ptr<const CharacterClass> character_class =
                CharacterClassParser::createCharacterClass(*character_class_json);
            if (controller.character_classes.find(character_class->name) != controller.character_classes.end()) {
                std::cerr << "Warning: Duplicate of character class \"" << character_class->name << "\" found in "
                          << entry.path() << ".\n";
            } else {
                controller.character_classes.emplace(character_class->name, std::move(character_class));
            }
        } catch (const nlohmann::json::out_of_range& e) {
            throw parsing_error("Character Class in file \"" + filename + "\" is missing an attribute.");
        } catch (const std::invalid_argument& e) {
            throw parsing_error("Character Class in file \"" + filename + "\": " + e.what());
        }
    }
}

void dnd::ContentParser::parseCharacterSubclasses(const std::filesystem::path& directory) {
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        const auto character_subclass_json = openJSON(entry);
        if (character_subclass_json == nullptr) {
            continue;
        }
        const std::string filename = entry.path().c_str();
        if (!character_subclass_json->is_object()) {
            throw parsing_error("Subclass file \"" + filename + "\" is not formatted as an object/map.");
        }
        try {
            std::shared_ptr<const CharacterSubclass> character_subclass =
                CharacterClassParser::createCharacterSubclass(*character_subclass_json);
            if (controller.character_subclasses.find(character_subclass->name)
                != controller.character_subclasses.end()) {
                std::cerr << "Warning: Duplicate of character subclass \"" << character_subclass->name << "\" found in "
                          << entry.path() << ".\n";
            } else {
                controller.character_subclasses.emplace(character_subclass->name, std::move(character_subclass));
            }
        } catch (const nlohmann::json::out_of_range& e) {
            throw parsing_error("Character Subclass in file \"" + filename + "\" is missing an attribute.");
        } catch (const std::invalid_argument& e) {
            throw parsing_error("Character Subclass in file \"" + filename + "\": " + e.what());
        }
    }
}

void dnd::ContentParser::parseCharacterRaces(const std::filesystem::path& directory) {
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        const auto character_race_json = openJSON(entry);
        if (character_race_json == nullptr) {
            continue;
        }
        const std::string filename = entry.path().c_str();
        if (!character_race_json->is_object()) {
            throw parsing_error("Race file \"" + filename + "\" is not formatted as an object/map.");
        }
        try {
            std::shared_ptr<const CharacterRace> character_race =
                CharacterRaceParser::createCharacterRace(*character_race_json);
            if (controller.character_races.find(character_race->name) != controller.character_races.end()) {
                std::cerr << "Warning: Duplicate of character race \"" << character_race->name << "\" found in "
                          << entry.path() << ".\n";
            } else {
                controller.character_races.emplace(character_race->name, std::move(character_race));
            }
        } catch (const nlohmann::json::out_of_range& e) {
            throw parsing_error("Character Race in file \"" + filename + "\" is missing an attribute.");
        } catch (const std::invalid_argument& e) {
            throw parsing_error("Character Race in file \"" + filename + "\": " + e.what());
        }
    }
}

void dnd::ContentParser::parseCharacterSubraces(const std::filesystem::path& directory) {
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        const auto character_subrace_json = openJSON(entry);
        if (character_subrace_json == nullptr) {
            continue;
        }
        const std::string filename = entry.path().c_str();
        if (!character_subrace_json->is_object()) {
            throw parsing_error("Subrace file \"" + filename + "\" is not formatted as an object/map.");
        }
        try {
            std::shared_ptr<const CharacterSubrace> character_subrace =
                CharacterRaceParser::createCharacterSubrace(*character_subrace_json);
            if (controller.character_subraces.find(character_subrace->name) != controller.character_subraces.end()) {
                std::cerr << "Warning: Duplicate of character subrace \"" << character_subrace->name << "\" found in "
                          << entry.path() << ".\n";
            } else {
                controller.character_subraces.emplace(character_subrace->name, std::move(character_subrace));
            }
        } catch (const nlohmann::json::out_of_range& e) {
            throw parsing_error("Character Subrace in file \"" + filename + "\" is missing an attribute.");
        } catch (const std::invalid_argument& e) {
            throw parsing_error("Character Subrace in file \"" + filename + "\": " + e.what());
        }
    }
}

void dnd::ContentParser::validateCharacterSubclasses() const {
    bool all_valid = true;
    for (const auto& [subclass_name, subclass_ptr] : controller.character_subclasses) {
        if (controller.character_classes.find(subclass_ptr->class_name) == controller.character_classes.cend()) {
            std::cerr << "Error: Subclass \"" << subclass_name << "\" is invalid. The class \""
                      << subclass_ptr->class_name << "\" does not exist.\n";
            all_valid = false;
        }
    }
    if (!all_valid) {
        throw parsing_error("Invalid character subclasses");
    }
}

void dnd::ContentParser::validateCharacterSubraces() const {
    bool all_valid = true;
    for (const auto& [subrace_name, subrace_ptr] : controller.character_subraces) {
        try {
            auto& race_ptr = controller.character_races.at(subrace_ptr->race_name);
            if (!race_ptr->has_subraces) {
                std::cerr << "Error: Subrace \"" << subrace_name << "\" is invalid. \"" << race_ptr->name
                          << "\" does not have subraces.\n";
                all_valid = false;
            }
        } catch (const std::out_of_range& e) {
            std::cerr << "Error: Subrace \"" << subrace_name << "\" is invalid. The race \"" << subrace_ptr->race_name
                      << "\" does not exist.\n";
            all_valid = false;
        }
    }
    if (!all_valid) {
        throw parsing_error("Invalid character subraces");
    }
}

void dnd::ContentParser::parseAll() {
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
    for (const auto& dir : std::filesystem::directory_iterator(content_path)) {
        if (!dir.is_directory()) {
            continue;
        }
        const std::string dir_name = dir.path().filename().c_str();
        if (dir_name != "general" && dir_name != campaign_dir_name) {
            continue;
        }
        std::cout << "Parsing \"" << dir_name << "\" directory...\n";
        for (const auto& sub_dir : std::filesystem::directory_iterator(dir)) {
            if (!sub_dir.is_directory()) {
                continue;
            }
            if (std::filesystem::exists(sub_dir.path() / "spells")) {
                parseSpells(sub_dir.path() / "spells");
            }
            if (std::filesystem::exists(sub_dir.path() / "classes")) {
                parseCharacterClasses(sub_dir.path() / "classes");
            }
            if (std::filesystem::exists(sub_dir.path() / "subclasses")) {
                parseCharacterSubclasses(sub_dir.path() / "subclasses");
            }
            if (std::filesystem::exists(sub_dir.path() / "races")) {
                parseCharacterRaces(sub_dir.path() / "races");
            }
            if (std::filesystem::exists(sub_dir.path() / "subraces")) {
                parseCharacterSubraces(sub_dir.path() / "subraces");
            }
        }
    }
    validateCharacterSubclasses();
    validateCharacterSubraces();
}
