#include "character_file_parser.hpp"

#include <array>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "models/character.hpp"
#include "models/character_class.hpp"
#include "models/character_race.hpp"
#include "models/spell.hpp"
#include "parsing/models/feature_holder_file_parser.hpp"
#include "parsing/parsing_exceptions.hpp"
#include "parsing/parsing_types.hpp"

void dnd::CharacterFileParser::parse() {
    if (!json_to_parse.is_object()) {
        throw json_format_error(ParsingType::CHARACTERS, filename, "map/object");
    }
    character_name = json_to_parse.at("name").get<std::string>();
    if (json_to_parse.at("base_ability_scores").size() != 6) {
        throw invalid_attribute(
            ParsingType::CHARACTERS, filename, "base_ability_scores", "must have exactly 6 entries."
        );
    }
    base_ability_scores = json_to_parse.at("base_ability_scores").get<std::array<int, 6>>();
    hit_dice_rolls = json_to_parse.at("hit_dice_rolls").get<std::vector<int>>();

    parseLevelAndXP();
    parseClassAndRace();

    if (json_to_parse.contains("features")) {
        parseFeatures();
    }
}

void dnd::CharacterFileParser::parseLevelAndXP() {
    const bool has_level = json_to_parse.contains("level");
    const bool has_xp = json_to_parse.contains("xp");
    if (has_level && has_xp) {
        level = json_to_parse.at("level").get<int>();
        if (level < 1 || level > 20) {
            throw invalid_attribute(ParsingType::CHARACTERS, filename, "level", "must be between 1 and 20.");
        }
        xp = json_to_parse.at("xp").get<int>();
        if (xp < 0) {
            throw invalid_attribute(ParsingType::CHARACTERS, filename, "xp", "must be positive.");
        }
        if (xp_for_level.at(level) > xp || (level < 20 && xp_for_level.at(level + 1) <= xp)) {
            throw invalid_attribute(
                ParsingType::CHARACTERS, filename, "xp",
                "corresponsds to a different level than the level value provided."
            );
        }
    } else if (has_level) {
        level = json_to_parse.at("level").get<int>();
        if (level < 1 || level > 20) {
            throw invalid_attribute(ParsingType::CHARACTERS, filename, "level", "must be between 1 and 20.");
        }
        xp = xp_for_level.at(level);
    } else if (has_xp) {
        xp = json_to_parse.at("xp").get<int>();
        if (xp < 0) {
            throw invalid_attribute(ParsingType::CHARACTERS, filename, "xp", "must be positive.");
        }
        level = Character::levelForXP(xp);
    } else {
        throw invalid_attribute(ParsingType::CHARACTERS, filename, "level/xp", "at least one must be provided.");
    }
}

void dnd::CharacterFileParser::parseClassAndRace() {
    const std::string character_class_name = json_to_parse.at("class").get<std::string>();
    try {
        class_ptr = character_classes.at(character_class_name);
    } catch (const std::out_of_range& e) {
        throw invalid_attribute(ParsingType::CHARACTERS, filename, "class", "does not exist");
    }

    if (json_to_parse.contains("subclass")) {
        const std::string character_subclass_name = json_to_parse.at("subclass").get<std::string>();
        try {
            subclass_ptr = character_subclasses.at(character_subclass_name);
        } catch (const std::out_of_range& e) {
            throw invalid_attribute(ParsingType::CHARACTERS, filename, "subclass", "does not exist");
        }
        if (class_ptr->subclass_level > level) {
            std::cerr << "Warning: Character " << character_name << " has subclass although the class \""
                      << class_ptr->name << "\" only allows subclasses from level " << class_ptr->subclass_level
                      << " on. Thus, the subclass will be ignored.\n";
        }
    } else if (class_ptr->subclass_level <= level) {
        throw attribute_missing(
            ParsingType::CHARACTERS, filename,
            "beginning at level " + std::to_string(class_ptr->subclass_level) + " a subclass is required for "
                + class_ptr->name + "s."
        );
    }

    const std::string character_race_name = json_to_parse.at("race").get<std::string>();
    try {
        race_ptr = character_races.at(character_race_name);
    } catch (const std::out_of_range& e) {
        throw invalid_attribute(ParsingType::CHARACTERS, filename, "race", "does not exist");
    }

    if (json_to_parse.contains("subrace")) {
        if (!race_ptr->has_subraces) {
            throw invalid_attribute(
                ParsingType::CHARACTERS, filename, "subrace",
                "is invalid because the race \"" + race_ptr->name + "\" has no subraces."
            );
        }
        const std::string character_subrace_name = json_to_parse.at("subrace").get<std::string>();
        try {
            subrace_ptr = character_subraces.at(character_subrace_name);
        } catch (const std::out_of_range& e) {
            throw invalid_attribute(ParsingType::CHARACTERS, filename, "subrace", "does not exist");
        }
    } else if (race_ptr->has_subraces) {
        throw attribute_missing(
            ParsingType::CHARACTERS, filename,
            "The race \"" + race_ptr->name
                + "\" requires a subrace selection."
                  "beginning at level "
                + std::to_string(class_ptr->subclass_level) + " a subclass is required for " + class_ptr->name + "s."
        );
    }
}

bool dnd::CharacterFileParser::validate() const {
    if (results.find(character_name) != results.end()) {
        std::cerr << "Warning: Duplicate of character \"" << character_name << "\" found.\n";
        return false;
    }
    return true;
}

void dnd::CharacterFileParser::saveResult() {
    // TODO: change Character constructor
    auto character = std::make_shared<Character>(character_name, base_ability_scores, level, xp);
    character->features = features;
    results.emplace(character_name, std::move(character));
}

void dnd::CharacterFileParser::reset() {
    FeatureHolderFileParser::reset();
    character_name = "";
    base_ability_scores = {0, 0, 0, 0, 0, 0};
    hit_dice_rolls = {};
    class_ptr = nullptr;
    subclass_ptr = nullptr;
    race_ptr = nullptr;
    subrace_ptr = nullptr;
    level = 0;
    xp = 0;
}
