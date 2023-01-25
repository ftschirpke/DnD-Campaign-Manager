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
#include "models/effect_holder/character_decision.hpp"
#include "models/feature_holder.hpp"
#include "models/spell.hpp"
#include "parsing/models/feature_holder_file_parser.hpp"
#include "parsing/parsing_exceptions.hpp"
#include "parsing/parsing_types.hpp"

void dnd::CharacterFileParser::parse() {
    DND_MEASURE_FUNCTION();
    if (!json_to_parse.is_object()) {
        throw json_format_error(ParsingType::CHARACTER, filepath, "map/object");
    }
    character_name = json_to_parse.at("name").get<std::string>();
    if (json_to_parse.at("base_ability_scores").size() != 6) {
        throw invalid_attribute(
            ParsingType::CHARACTER, filepath, "base_ability_scores", "must have exactly 6 entries."
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

    if (json_to_parse.contains("decisions")) {
        if (!json_to_parse.at("decisions").is_object()) {
            throw attribute_format_error(ParsingType::CHARACTER, filepath, "decisions", "map/object");
        }
        for (const auto& [feature_name, feature_decisions] : json_to_parse.at("decisions").items()) {
            if (!feature_decisions.is_object()) {
                throw attribute_format_error(
                    ParsingType::CHARACTER, filepath, "decisions:" + feature_name, "map/object"
                );
            }
            parseCharacterDecisions(feature_name, feature_decisions);
        }
    }
}

void dnd::CharacterFileParser::parseCharacterDecisions(
    const std::string& feature_name, const nlohmann::json& feature_decisions_json
) {
    const Feature* feature_ptr = nullptr;
    const std::vector<const FeatureHolder*> feature_holders{class_ptr, subclass_ptr, race_ptr, subrace_ptr};
    for (auto feature_holder : feature_holders) {
        for (const auto& feature : feature_holder->features) {
            if (feature.name == feature_name) {
                feature_ptr = &feature;
                break;
            }
        }
        if (feature_ptr != nullptr) {
            break;
        }
    }
    if (feature_ptr == nullptr) {
        throw invalid_attribute(
            ParsingType::CHARACTER, filepath, "decision:" + feature_name, "no such feature exists."
        );
    }
    // TODO: the feature might also be a choosable i.e. an eldritch invocation or feat

    if (feature_ptr->parts_with_choices.empty()) {
        throw invalid_attribute(
            ParsingType::CHARACTER, filepath, "decision:" + feature_name, "the feature has no choices."
        );
    }
    for (const auto& eh_with_choices : feature_ptr->parts_with_choices) {
        bool found = false;
        for (auto it = eh_with_choices.choices.cbegin(); it != eh_with_choices.choices.cend(); ++it) {
            // TODO: find the matching choice
        }
    }
}

// dnd::CharacterDecision dnd::CharacterFileParser::createCharacterDecision(
//     const std::string& feature_name, const nlohmann::json& decision_json
// ) {}

void dnd::CharacterFileParser::parseLevelAndXP() {
    DND_MEASURE_FUNCTION();
    const bool has_level = json_to_parse.contains("level");
    const bool has_xp = json_to_parse.contains("xp");
    if (has_level && has_xp) {
        level = json_to_parse.at("level").get<int>();
        if (level < 1 || level > 20) {
            throw invalid_attribute(ParsingType::CHARACTER, filepath, "level", "must be between 1 and 20.");
        }
        xp = json_to_parse.at("xp").get<int>();
        if (xp < 0) {
            throw invalid_attribute(ParsingType::CHARACTER, filepath, "xp", "must be positive.");
        }
        if (xp_for_level.at(level) > xp || (level < 20 && xp_for_level.at(level + 1) <= xp)) {
            throw invalid_attribute(
                ParsingType::CHARACTER, filepath, "xp",
                "corresponsds to a different level than the level value provided."
            );
        }
    } else if (has_level) {
        level = json_to_parse.at("level").get<int>();
        if (level < 1 || level > 20) {
            throw invalid_attribute(ParsingType::CHARACTER, filepath, "level", "must be between 1 and 20.");
        }
        xp = xp_for_level.at(level);
    } else if (has_xp) {
        xp = json_to_parse.at("xp").get<int>();
        if (xp < 0) {
            throw invalid_attribute(ParsingType::CHARACTER, filepath, "xp", "must be positive.");
        }
        level = Character::levelForXP(xp);
    } else {
        throw invalid_attribute(ParsingType::CHARACTER, filepath, "level/xp", "at least one must be provided.");
    }
}

void dnd::CharacterFileParser::parseClassAndRace() {
    DND_MEASURE_FUNCTION();
    const std::string character_class_name = json_to_parse.at("class").get<std::string>();
    try {
        class_ptr = &character_classes.at(character_class_name);
    } catch (const std::out_of_range& e) {
        throw invalid_attribute(
            ParsingType::CHARACTER, filepath, "class", '\"' + character_class_name + "\" does not exist"
        );
    }

    if (json_to_parse.contains("subclass")) {
        const std::string character_subclass_name = json_to_parse.at("subclass").get<std::string>();
        try {
            subclass_ptr = &character_subclasses.at(character_subclass_name);
        } catch (const std::out_of_range& e) {
            throw invalid_attribute(
                ParsingType::CHARACTER, filepath, "subclass", '\"' + character_subclass_name + "\" does not exist"
            );
        }
        if (class_ptr->subclass_level > level) {
            std::cerr << "Warning: Character " << character_name << " has subclass although the class \""
                      << class_ptr->name << "\" only allows subclasses from level " << class_ptr->subclass_level
                      << " on. Thus, the subclass will be ignored.\n";
        }
    } else if (class_ptr->subclass_level <= level) {
        throw attribute_missing(
            ParsingType::CHARACTER, filepath,
            "beginning at level " + std::to_string(class_ptr->subclass_level) + " a subclass is required for "
                + class_ptr->name + "s."
        );
    }

    const std::string character_race_name = json_to_parse.at("race").get<std::string>();
    try {
        race_ptr = &character_races.at(character_race_name);
    } catch (const std::out_of_range& e) {
        throw invalid_attribute(
            ParsingType::CHARACTER, filepath, "race", '\"' + character_race_name + "\" does not exist"
        );
    }

    if (json_to_parse.contains("subrace")) {
        if (!race_ptr->has_subraces) {
            throw invalid_attribute(
                ParsingType::CHARACTER, filepath, "subrace",
                "is invalid because the race \"" + race_ptr->name + "\" has no subraces."
            );
        }
        const std::string character_subrace_name = json_to_parse.at("subrace").get<std::string>();
        try {
            subrace_ptr = &character_subraces.at(character_subrace_name);
        } catch (const std::out_of_range& e) {
            throw invalid_attribute(
                ParsingType::CHARACTER, filepath, "subrace", '\"' + character_subrace_name + "\" does not exist"
            );
        }
    } else if (race_ptr->has_subraces) {
        std::cout << "JSON:\n" << json_to_parse << std::endl;
        throw attribute_missing(
            ParsingType::CHARACTER, filepath, "The race \"" + race_ptr->name + "\" requires a subrace selection."
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
    character.features = retrieveFeatures();
    character.race_ptr = race_ptr;
    character.subrace_ptr = subrace_ptr;
    character.class_ptr = class_ptr;
    character.subclass_ptr = subclass_ptr;
}
