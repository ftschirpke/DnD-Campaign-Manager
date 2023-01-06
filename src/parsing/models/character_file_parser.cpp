#include "dnd_config.hpp"

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
    DND_MEASURE_FUNCTION();
    if (!json_to_parse.is_object()) {
        throw json_format_error(ParsingType::CHARACTER, filename, "map/object");
    }
    character_name = json_to_parse.at("name").get<std::string>();
    if (json_to_parse.at("base_ability_scores").size() != 6) {
        throw invalid_attribute(
            ParsingType::CHARACTER, filename, "base_ability_scores", "must have exactly 6 entries."
        );
    }
    base_ability_scores = json_to_parse.at("base_ability_scores").get<std::array<int, 6>>();
    hit_dice_rolls = json_to_parse.at("hit_dice_rolls").get<std::vector<int>>();

    parseLevelAndXP();
    parseClassAndRace();

    // TODO: parse spells

    if (json_to_parse.contains("features")) {
        try {
            parseFeatures();
        } catch (parsing_error& e) {
            e.setParsingType(ParsingType::CHARACTER);
            throw e;
        }
    }
}

void dnd::CharacterFileParser::parseLevelAndXP() {
    const bool has_level = json_to_parse.contains("level");
    const bool has_xp = json_to_parse.contains("xp");
    if (has_level && has_xp) {
        level = json_to_parse.at("level").get<int>();
        if (level < 1 || level > 20) {
            throw invalid_attribute(ParsingType::CHARACTER, filename, "level", "must be between 1 and 20.");
        }
        xp = json_to_parse.at("xp").get<int>();
        if (xp < 0) {
            throw invalid_attribute(ParsingType::CHARACTER, filename, "xp", "must be positive.");
        }
        if (xp_for_level.at(level) > xp || (level < 20 && xp_for_level.at(level + 1) <= xp)) {
            throw invalid_attribute(
                ParsingType::CHARACTER, filename, "xp",
                "corresponsds to a different level than the level value provided."
            );
        }
    } else if (has_level) {
        level = json_to_parse.at("level").get<int>();
        if (level < 1 || level > 20) {
            throw invalid_attribute(ParsingType::CHARACTER, filename, "level", "must be between 1 and 20.");
        }
        xp = xp_for_level.at(level);
    } else if (has_xp) {
        xp = json_to_parse.at("xp").get<int>();
        if (xp < 0) {
            throw invalid_attribute(ParsingType::CHARACTER, filename, "xp", "must be positive.");
        }
        level = Character::levelForXP(xp);
    } else {
        throw invalid_attribute(ParsingType::CHARACTER, filename, "level/xp", "at least one must be provided.");
    }
}

void dnd::CharacterFileParser::parseClassAndRace() {
    const std::string character_class_name = json_to_parse.at("class").get<std::string>();
    try {
        class_ptr = &character_classes.at(character_class_name);
    } catch (const std::out_of_range& e) {
        throw invalid_attribute(
            ParsingType::CHARACTER, filename, "class", '\"' + character_class_name + "\" does not exist"
        );
    }

    if (json_to_parse.contains("subclass")) {
        const std::string character_subclass_name = json_to_parse.at("subclass").get<std::string>();
        try {
            subclass_ptr = &character_subclasses.at(character_subclass_name);
        } catch (const std::out_of_range& e) {
            throw invalid_attribute(
                ParsingType::CHARACTER, filename, "subclass", '\"' + character_subclass_name + "\" does not exist"
            );
        }
        if (class_ptr->subclass_level > level) {
            std::cerr << "Warning: Character " << character_name << " has subclass although the class \""
                      << class_ptr->name << "\" only allows subclasses from level " << class_ptr->subclass_level
                      << " on. Thus, the subclass will be ignored.\n";
        }
    } else if (class_ptr->subclass_level <= level) {
        throw attribute_missing(
            ParsingType::CHARACTER, filename,
            "beginning at level " + std::to_string(class_ptr->subclass_level) + " a subclass is required for "
                + class_ptr->name + "s."
        );
    }

    const std::string character_race_name = json_to_parse.at("race").get<std::string>();
    try {
        race_ptr = &character_races.at(character_race_name);
    } catch (const std::out_of_range& e) {
        throw invalid_attribute(
            ParsingType::CHARACTER, filename, "race", '\"' + character_race_name + "\" does not exist"
        );
    }

    if (json_to_parse.contains("subrace")) {
        if (!race_ptr->has_subraces) {
            throw invalid_attribute(
                ParsingType::CHARACTER, filename, "subrace",
                "is invalid because the race \"" + race_ptr->name + "\" has no subraces."
            );
        }
        const std::string character_subrace_name = json_to_parse.at("subrace").get<std::string>();
        try {
            subrace_ptr = &character_subraces.at(character_subrace_name);
        } catch (const std::out_of_range& e) {
            throw invalid_attribute(
                ParsingType::CHARACTER, filename, "subrace", '\"' + character_subrace_name + "\" does not exist"
            );
        }
    } else if (race_ptr->has_subraces) {
        std::cout << "JSON:\n" << json_to_parse << std::endl;
        throw attribute_missing(
            ParsingType::CHARACTER, filename, "The race \"" + race_ptr->name + "\" requires a subrace selection."
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
    results.emplace(
        std::piecewise_construct, std::forward_as_tuple(character_name),
        std::forward_as_tuple(character_name, base_ability_scores, level, xp, hit_dice_rolls)
    );
    Character& character = results.at(character_name);
    character.features = std::move(features);
    character.race_ptr = race_ptr;
    character.subrace_ptr = subrace_ptr;
    character.class_ptr = class_ptr;
    character.subclass_ptr = subclass_ptr;
}
