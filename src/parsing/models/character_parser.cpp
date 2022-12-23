#include "character_parser.hpp"

#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

#include "controllers/content_controller.hpp"
#include "models/character.hpp"


std::shared_ptr<dnd::Character> dnd::CharacterParser::createCharacter(
    const nlohmann::json& character_json, const ContentController& content_controller
) {
    const std::string character_name = character_json.at("name").get<std::string>();
    const std::vector<int> base_ability_scores = character_json.at("base_ability_scores").get<std::vector<int>>();
    if (base_ability_scores.size() != 6) {
        std::stringstream sstr;
        sstr << "Character \"" << character_name << "\" is invalid. Characters must have exactly 6 ability scores\n";
        throw std::invalid_argument(sstr.str());
    }
    Character character(character_name, base_ability_scores);

    parseFeatureHolders(character, character_json, content_controller);
    parseLevelAndXP(character, character_json);

    const std::vector<int> hit_dice_rolls = character_json.at("hit_dice_rolls").get<std::vector<int>>();
    // TODO: consider a better way of adding the hit dice rolls
    for (int dice_roll : hit_dice_rolls) {
        character.addHitDiceRoll(dice_roll);
    }

    if (character.getLevel() != hit_dice_rolls.size()) {
        std::cerr << "Warning: Character \"" << character.name << "\" is incomplete. "
                  << "Amount of hit dice rolls does not equal character level.\n";
    }

    // TODO: calculate CharacterState
    // TODO: parse characters spellcasting
    // TODO: parse characters equipment
    // TODO: parse characters custom background
    // TODO: parse characters custom spells
    // TODO: parse characters custom features
    // TODO: add unused values as map to character?

    return std::make_shared<Character>(std::move(character));
}

void dnd::CharacterParser::parseFeatureHolders(
    dnd::Character& character, const nlohmann::json& character_json, const dnd::ContentController& content_controller
) {
    const std::string character_class_name = character_json.at("class").get<std::string>();
    try {
        character.class_ptr = content_controller.character_classes.at(character_class_name);
    } catch (const std::out_of_range& e) {
        std::stringstream sstr;
        sstr << "Character \"" << character.name << "\" is invalid. The class \"" << character_class_name
             << "\" does not exist.\n";
        throw std::invalid_argument(sstr.str());
    }

    if (character_json.contains("subclass")) {
        const std::string character_subclass_name = character_json.at("subclass").get<std::string>();
        try {
            character.subclass_ptr = content_controller.character_subclasses.at(character_subclass_name);
        } catch (const std::out_of_range& e) {
            std::stringstream sstr;
            sstr << "Character \"" << character.name << "\" is invalid. The subclass \"" << character_subclass_name
                 << "\" does not exist.\n";
            throw std::invalid_argument(sstr.str());
        }
    } else {
        // TODO: check whether character needs subclass -> and then throw error, if character does not have one
        // this requires activation parsing first
        // and then attribute of CharacterClass?
    }

    const std::string character_race_name = character_json.at("race").get<std::string>();
    try {
        character.race_ptr = content_controller.character_races.at(character_race_name);
    } catch (const std::out_of_range& e) {
        std::stringstream sstr;
        sstr << "Character \"" << character.name << "\" is invalid. The race \"" << character_race_name
             << "\" does not exist.\n";
        throw std::invalid_argument(sstr.str());
    }

    if (character_json.contains("subrace")) {
        const std::string character_subrace_name = character_json.at("subrace").get<std::string>();
        try {
            character.subrace_ptr = content_controller.character_subraces.at(character_subrace_name);
        } catch (const std::out_of_range& e) {
            std::stringstream sstr;
            sstr << "Character \"" << character.name << "\" is invalid. The subrace \"" << character_subrace_name
                 << "\" does not exist.\n";
            throw std::invalid_argument(sstr.str());
        }
    } else if (character.race_ptr->has_subraces) {
        std::stringstream sstr;
        sstr << "Character \"" << character.name << "\" is invalid. The race \"" << character.race_ptr->name
             << "\" requires a subrace selection.\n";
        throw std::invalid_argument(sstr.str());
    }
}

void dnd::CharacterParser::checkLevel(const std::string& character_name, int level) {
    if (level < 1 || level > 20) {
        std::stringstream sstr;
        sstr << "Character \"" << character_name << "\" is invalid. Level must be between 1 and 20.\n";
        throw std::invalid_argument(sstr.str());
    }
}

void dnd::CharacterParser::checkXP(const std::string& character_name, int xp) {
    if (xp < 0) {
        std::stringstream sstr;
        sstr << "Character \"" << character_name << "\" is invalid. XP must be positive.\n";
        throw std::invalid_argument(sstr.str());
    }
}

void dnd::CharacterParser::parseLevelAndXP(dnd::Character& character, const nlohmann::json& character_json) {
    const bool has_level = character_json.contains("level");
    const bool has_xp = character_json.contains("xp");
    if (has_level && has_xp) {
        const int level = character_json.at("level").get<int>();
        checkLevel(character.name, level);
        const int xp = character_json.at("xp").get<int>();
        checkXP(character.name, xp);
        if (xp_for_level.at(level) > xp || (level < 20 && xp_for_level.at(level + 1) <= xp)) {
            std::stringstream sstr;
            sstr << "Character \"" << character.name << "\" is invalid. Level and XP values are contradictory.\n";
            throw std::invalid_argument(sstr.str());
        }
        character.setXP(xp);
    } else if (has_level) {
        const int level = character_json.at("level").get<int>();
        checkLevel(character.name, level);
        character.setLevel(level);
    } else if (has_xp) {
        const int xp = character_json.at("xp").get<int>();
        checkXP(character.name, xp);
        character.setXP(xp);
    } else {
        std::stringstream sstr;
        sstr << "Character \"" << character.name << "\" is invalid. A character requires level or XP value.\n";
        throw std::invalid_argument(sstr.str());
    }
}
