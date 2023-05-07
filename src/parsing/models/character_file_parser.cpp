#include "dnd_config.hpp"

#include "character_file_parser.hpp"

#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include "models/character.hpp"
#include "models/character_class.hpp"
#include "models/character_race.hpp"
#include "models/effect_holder/character_decision.hpp"
#include "models/effect_holder/choice.hpp"
#include "models/effect_holder/effect_holder_with_choices.hpp"
#include "models/effect_holder/feature.hpp"
#include "models/feature_holder.hpp"
#include "parsing/content_file_parser.hpp"
#include "parsing/models/effect_holder/effect_holder_parser.hpp"
#include "parsing/models/effect_holder/features_parser.hpp"
#include "parsing/parsing_exceptions.hpp"
#include "parsing/parsing_types.hpp"

void dnd::CharacterFileParser::parse() {
    DND_MEASURE_FUNCTION();
    if (!json_to_parse.is_object()) {
        throw json_format_error(type, filepath, "map/object");
    }

    character_name = json_to_parse.at("name").get<std::string>();

    if (json_to_parse.at("base_ability_scores").size() != 6) {
        throw invalid_attribute(type, filepath, "base_ability_scores", "must have exactly 6 entries.");
    }

    base_ability_scores = json_to_parse.at("base_ability_scores").get<std::array<int, 6>>();

    auto not_between_one_and_twenty = [&](int x) { return (x <= 0 || x > 20); };
    if (std::any_of(base_ability_scores.begin(), base_ability_scores.end(), not_between_one_and_twenty)) {
        throw invalid_attribute(type, filepath, "base_ability_scores", "all entries must be between 1 and 20");
    }

    parseLevelAndXP();
    parseClassAndRace();

    hit_dice_rolls = json_to_parse.at("hit_dice_rolls").get<std::vector<int>>();

    auto not_between_one_and_dicemax = [&](int x) { return (x <= 0 || x > diceToInt(class_ptr->hit_dice)); };
    if (std::any_of(hit_dice_rolls.begin(), hit_dice_rolls.end(), not_between_one_and_dicemax)) {
        throw invalid_attribute(
            type, filepath, "hit_dice_rolls",
            "all entries must be valid values for the hit dice (" + diceToString(class_ptr->hit_dice) + ')'
        );
    }

    // TODO: parse spells
    DND_UNUSED(spells);

    if (json_to_parse.contains("features")) {
        features_parser.parseFeatures(json_to_parse.at("features"));
    }

    if (json_to_parse.contains("decisions")) {
        if (!json_to_parse.at("decisions").is_object()) {
            throw attribute_format_error(type, filepath, "decisions", "map/object");
        }
        for (const auto& [feature_name, feature_decisions] : json_to_parse.at("decisions").items()) {
            if (!feature_decisions.is_object()) {
                throw attribute_format_error(type, filepath, "decisions:" + feature_name, "map/object");
            }
            parseCharacterDecisions(feature_name, feature_decisions);
        }
    }
}

static const dnd::Feature* determineFeature(
    const std::string& feature_name, const std::vector<const dnd::FeatureHolder*> feature_holders
) {
    const dnd::Feature* feature_ptr = nullptr;
    for (auto feature_holder : feature_holders) {
        if (feature_holder == nullptr) {
            continue;
        }
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
    return feature_ptr;
}

static const dnd::Choice* determineChoice(
    const std::vector<dnd::CharacterDecision>& parsed_decisions, const std::string& attribute_name,
    const nlohmann::json& decision_json, const std::vector<dnd::EffectHolderWithChoices>& ehs_with_choices
) {
    for (const auto& eh : ehs_with_choices) {
        for (auto choice_it = eh.choices.cbegin(); choice_it != eh.choices.cend(); ++choice_it) {
            auto decision_has_choice = [&](const dnd::CharacterDecision& decision) {
                return decision.choice == choice_it->get();
            };
            if (std::any_of(parsed_decisions.cbegin(), parsed_decisions.cend(), decision_has_choice)) {
                // skip this choice if a decision already belongs to it
                continue;
            }

            if (attribute_name != (*choice_it)->attribute_name) {
                continue;
            }

            auto is_valid_decision = [&](const nlohmann::json& chosen_val) -> bool {
                return (*choice_it)->isValidDecision(chosen_val.get<std::string>());
            };
            if (std::all_of(decision_json.cbegin(), decision_json.cend(), is_valid_decision)) {
                return choice_it->get();
            }
        }
    }
    return nullptr;
}

void dnd::CharacterFileParser::parseCharacterDecisions(
    const std::string& feature_name, const nlohmann::json& feature_decisions_json
) {
    std::vector<const FeatureHolder*> feature_holders{class_ptr, race_ptr};
    if (subclass_ptr != nullptr) {
        feature_holders.emplace_back(subclass_ptr);
    }
    if (subrace_ptr != nullptr) {
        feature_holders.emplace_back(subrace_ptr);
    }

    const Feature* feature_ptr = determineFeature(feature_name, feature_holders);

    if (feature_ptr == nullptr) {
        throw invalid_attribute(type, filepath, "decision", "no feature \"" + feature_name + "\" exists.");
    }
    // TODO: the feature might also be a choosable i.e. an eldritch invocation or feat

    if (feature_ptr->parts_with_choices.empty()) {
        throw invalid_attribute(type, filepath, "decision:" + feature_name, "the feature has no choices.");
    }

    decisions.reserve(feature_decisions_json.size());

    for (const auto& [attribute_name, decision_json] : feature_decisions_json.items()) {
        if (!decision_json.is_array()) {
            throw attribute_format_error(type, filepath, "decision:" + feature_name, "array");
        }

        const Choice* determined_choice = determineChoice(
            decisions, attribute_name, decision_json, feature_ptr->parts_with_choices
        );
        if (determined_choice == nullptr) {
            throw invalid_attribute(
                type, filepath, "decision:" + feature_name + ':' + attribute_name,
                "is not valid for any choice of \"" + feature_name + "\"."
            );
        }

        CharacterDecision new_decision(determined_choice);
        effect_holder_parser.parseEffectHolder(
            nlohmann::json::object({{attribute_name, decision_json}}), &new_decision.decision_effects
        );
        decisions.emplace_back(std::move(new_decision));
    }
}

void dnd::CharacterFileParser::parseLevelAndXP() {
    DND_MEASURE_FUNCTION();
    const bool has_level = json_to_parse.contains("level");
    const bool has_xp = json_to_parse.contains("xp");
    if (has_level && has_xp) {
        level = json_to_parse.at("level").get<int>();
        if (level < 1 || level > 20) {
            throw invalid_attribute(type, filepath, "level", "must be between 1 and 20.");
        }
        xp = json_to_parse.at("xp").get<int>();
        if (Character::minxp_for_level.at(static_cast<size_t>(level - 1)) > xp
            || (level < 20 && Character::minxp_for_level.at(static_cast<size_t>(level)) <= xp)) {
            throw invalid_attribute(
                type, filepath, "xp", "corresponds to a different level than the level value provided."
            );
        }
    } else if (has_level) {
        level = json_to_parse.at("level").get<int>();
        if (level < 1 || level > 20) {
            throw invalid_attribute(type, filepath, "level", "must be between 1 and 20.");
        }
        xp = Character::minxp_for_level.at(static_cast<size_t>(level - 1));
    } else if (has_xp) {
        xp = json_to_parse.at("xp").get<int>();
        level = Character::levelForXP(xp);
    } else {
        throw invalid_attribute(type, filepath, "level/xp", "at least one must be provided.");
    }
}

void dnd::CharacterFileParser::parseClassAndRace() {
    DND_MEASURE_FUNCTION();
    const std::string character_class_name = json_to_parse.at("class").get<std::string>();
    try {
        class_ptr = &character_classes.at(character_class_name);
    } catch (const std::out_of_range& e) {
        DND_UNUSED(e);
        throw invalid_attribute(type, filepath, "class", '\"' + character_class_name + "\" does not exist");
    }

    if (json_to_parse.contains("subclass")) {
        const std::string character_subclass_name = json_to_parse.at("subclass").get<std::string>();
        try {
            subclass_ptr = &character_subclasses.at(character_subclass_name);
        } catch (const std::out_of_range& e) {
            DND_UNUSED(e);
            throw invalid_attribute(type, filepath, "subclass", '\"' + character_subclass_name + "\" does not exist");
        }
        if (class_ptr->subclass_level > level) {
            std::cerr << "Warning: Character " << character_name << " has subclass although the class \""
                      << class_ptr->name << "\" only allows subclasses from level " << class_ptr->subclass_level
                      << " on. Thus, the subclass will be ignored.\n";
        }
    } else if (class_ptr->subclass_level <= level) {
        throw attribute_missing(
            type, filepath,
            "beginning at level " + std::to_string(class_ptr->subclass_level) + " a subclass is required for "
                + class_ptr->name + "s."
        );
    }

    const std::string character_race_name = json_to_parse.at("race").get<std::string>();
    try {
        race_ptr = &character_races.at(character_race_name);
    } catch (const std::out_of_range& e) {
        DND_UNUSED(e);
        throw invalid_attribute(type, filepath, "race", '\"' + character_race_name + "\" does not exist");
    }

    if (json_to_parse.contains("subrace")) {
        if (!race_ptr->has_subraces) {
            throw invalid_attribute(
                type, filepath, "subrace", "is invalid because the race \"" + race_ptr->name + "\" has no subraces."
            );
        }
        const std::string character_subrace_name = json_to_parse.at("subrace").get<std::string>();
        try {
            subrace_ptr = &character_subraces.at(character_subrace_name);
        } catch (const std::out_of_range& e) {
            DND_UNUSED(e);
            throw invalid_attribute(type, filepath, "subrace", '\"' + character_subrace_name + "\" does not exist");
        }
    } else if (race_ptr->has_subraces) {
        throw attribute_missing(type, filepath, "The race \"" + race_ptr->name + "\" requires a subrace selection.");
    }
}

bool dnd::CharacterFileParser::validate() const {
    if (characters.contains(character_name)) {
        std::cerr << "Warning: Duplicate of character \"" << character_name << "\" found.\n";
        return false;
    }
    return true;
}

void dnd::CharacterFileParser::saveResult() {
    // TODO: change Character constructor
    characters.emplace(
        std::piecewise_construct, std::forward_as_tuple(character_name),
        std::forward_as_tuple(
            character_name, std::move(features_parser.retrieveFeatures()), base_ability_scores, level, xp,
            hit_dice_rolls
        )
    );
    Character& character = characters.at(character_name);
    character.decisions = std::move(decisions);
    character.race_ptr = race_ptr;
    character.subrace_ptr = subrace_ptr;
    character.class_ptr = class_ptr;
    character.subclass_ptr = subclass_ptr;
}
