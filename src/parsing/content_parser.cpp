#include "content_parser.hpp"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>

#include <nlohmann/json.hpp>

#include "models/character_race_parser.hpp"
#include "models/spell_parser.hpp"
#include "../controllers/content_controller.hpp"
#include "../models/spell.hpp"

const std::unique_ptr<const nlohmann::json> dnd::ContentParser::openJSON(const std::filesystem::directory_entry& file) {
    if (!file.is_regular_file()) {
        std::cerr << "Warning: " << file.path()
            << " is not a regular file.\n";
        return nullptr;
    }
    const std::string path = file.path().c_str();
    if (path.compare(path.length()-5, path.length(), ".json") != 0) {
        std::cerr << "Warning: \"" << path
            << "\" is not a \".json\" file.\n";
        return nullptr;
    }
    std::ifstream json_file(path);
    nlohmann::json j;
    try {
        json_file >> j;
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Warning: ParseError occured while parsing \"" << path << "\":\n"
            << e.what() << '\n';
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
            throw parsing_error(
                "Spell file \"" + filename + "\" is not formatted as an object/map."
            );
        }
        for (const auto& [spell_name, spell_info] : spells_json->items()) {
            try {
                std::unique_ptr<Spell> spell = SpellParser::createSpell(spell_name, spell_info);
                if (controller.spells.find(spell_name) != controller.spells.end()) {
                    std::cerr << "Warning: Duplicate of spell \""
                        << spell_name << "\" found in "
                        << entry.path() << ".\n";
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

void dnd::ContentParser::parseCharacterRaces(const std::filesystem::path& directory) {
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        const auto character_race_json = openJSON(entry);
        if (character_race_json == nullptr) {
            continue;
        }
        const std::string filename = entry.path().c_str();
        if (!character_race_json->is_object()) {
            throw parsing_error(
                "Race file \"" + filename + "\" is not formatted as an object/map."
            );
        }
        try {
            std::unique_ptr<CharacterRace> character_race = \
                CharacterRaceParser::createCharacterRace(*character_race_json);
            if (controller.character_races.find(character_race->name) 
                != controller.character_races.end()) {
                std::cerr << "Warning: Duplicate of character race \""
                    << character_race->name << "\" found in "
                    << entry.path() << ".\n";
            } else {
                controller.character_races.emplace(character_race->name, std::move(character_race));
            }
        } catch (const nlohmann::json::out_of_range& e) {
            throw parsing_error(
                "Character Race in file \"" + filename + "\" is missing an attribute."
            );
        } catch (const std::invalid_argument& e) {
            throw parsing_error("Character Race in file \"" + filename + "\": " + e.what());
        }
    }
}

void dnd::ContentParser::parseAll() {
    const std::filesystem::path path(content_path);
    try {
        std::filesystem::directory_entry entry(path);
        if (!entry.is_directory()) {
            std::stringstream sstr;
            sstr << path << " is not a directory.";
            throw std::invalid_argument(sstr.str());
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::stringstream sstr;
        sstr << path << " does not exist.";
        throw std::invalid_argument(sstr.str());
    }
    for (const auto& dir : std::filesystem::directory_iterator(content_path)) {
        if (!dir.is_directory()) {
            continue;
        }
        for (const auto& sub_dir : std::filesystem::directory_iterator(dir)) {
            if (!sub_dir.is_directory()) {
                continue;
            }
            if (std::filesystem::exists(sub_dir.path()/"spells")) {
                parseSpells(sub_dir.path()/"spells");
            }
            if (std::filesystem::exists(sub_dir.path()/"races")) {
                parseCharacterRaces(sub_dir.path()/"races");
            }
        }
    }
}
