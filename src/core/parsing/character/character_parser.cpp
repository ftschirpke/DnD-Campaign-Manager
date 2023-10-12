#include <dnd_config.hpp>

#include "character_parser.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/models/character/character.hpp>
#include <core/models/feature/feature.hpp>
#include <core/parsing/feature/feature_parser.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/validation/character/character_data.hpp>
#include <core/validation/character/decision/decision_data.hpp>

dnd::CharacterParser::CharacterParser(const std::filesystem::path& filepath) noexcept
    : FileParser(filepath), feature_parser(filepath) {}

dnd::Errors dnd::CharacterParser::parse() {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(
            ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(), "The character json is not an object."
        );
    }

    errors += parse_required_attribute(json, "name", data.name);
    errors += parse_required_attribute(json, "description", data.description);
    data.source_path = get_filepath();

    errors += parse_required_attribute(json, "base_ability_scores", data.base_ability_scores_data.ability_scores);
    errors += parse_required_attribute(json, "race", data.character_basis_data.race_name);
    errors += parse_required_attribute(json, "class", data.character_basis_data.class_name);
    errors += parse_optional_attribute(json, "subrace", data.character_basis_data.subrace_name);
    errors += parse_optional_attribute(json, "subclass", data.character_basis_data.subclass_name);

    bool has_level = json.contains("level");
    bool has_xp = json.contains("xp");
    if (!has_level && !has_xp) {
        errors.add_parsing_error(ParsingErrorCode::MISSING_ATTRIBUTE, get_filepath(), "Character has no level or xp.");
    } else {
        errors += parse_optional_attribute(json, "level", data.progression_data.level);
        errors += parse_optional_attribute(json, "xp", data.progression_data.xp);
    }
    errors += parse_required_attribute(json, "hit_dice_rolls", data.progression_data.hit_dice_rolls);

    if (json.contains("decisions")) {
        if (!json["decisions"].is_object()) {
            errors.add_parsing_error(
                ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(), "The decisions json is not an object."
            );
        } else {
            for (auto& [feature_name, decisions_for_feature] : json["decisions"].items()) {
                if (decisions_for_feature.is_array()) {
                    for (auto& decision_json : decisions_for_feature) {
                        DecisionData& decision_data = data.decisions_data.emplace_back(&data, nullptr);
                        decision_data.feature_name = feature_name;
                        errors += parse_decision(std::move(decision_json), decision_data);
                    }
                } else {
                    DecisionData& decision_data = data.decisions_data.emplace_back(&data, nullptr);
                    decision_data.feature_name = feature_name;
                    errors += parse_decision(std::move(decisions_for_feature), decision_data);
                }
            }
        }
    }

    if (json.contains("features")) {
        errors += feature_parser.parse_multiple(std::move(json["features"]), data.features_data, &data);
    }

    return errors;
}

void dnd::CharacterParser::set_context(const dnd::Content& content) {
    std::set<const EffectHolder*> processed_effect_holders;
    for (auto& decision_data : data.decisions_data) {
        const Feature* feature;
        if (content.get_features().contains(decision_data.feature_name)) {
            feature = &content.get_features().get(decision_data.feature_name);
        } else if (content.get_choosables().contains(decision_data.feature_name)) {
            feature = &content.get_choosables().get(decision_data.feature_name);
        } else {
            decision_data.set_target(nullptr);
            continue;
        }
        std::vector<const EffectHolder*> effect_holders_with_choices;
        if (!feature->get_main_part().get_choices().empty()) {
            effect_holders_with_choices.emplace_back(&feature->get_main_part());
        }
        const ClassFeature* class_feature = dynamic_cast<const ClassFeature*>(feature); // TODO: temporary
        if (class_feature != nullptr) {
            for (const auto& effect_holder : class_feature->get_higher_level_parts()) {
                if (!effect_holder.get_choices().empty()) {
                    effect_holders_with_choices.emplace_back(&effect_holder);
                }
            }
        }
        for (const EffectHolder* effect_holder : effect_holders_with_choices) {
            if (processed_effect_holders.contains(effect_holder)) {
                continue;
            }
            decision_data.set_target(effect_holder);
            processed_effect_holders.emplace(effect_holder);
        }
    }
}

dnd::Errors dnd::CharacterParser::validate(const Content& content) const {
    Errors errors = data.validate();
    errors += data.validate_relations(content);
    return errors;
}

void dnd::CharacterParser::save_result(Content& content) {
    content.add_character(Character::create(std::move(data), content));
}

dnd::Errors dnd::CharacterParser::parse_decision(
    nlohmann::ordered_json&& decision_json, dnd::DecisionData& decision_data
) const {
    Errors errors;
    if (!decision_json.is_object()) {
        errors.add_parsing_error(
            ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(), "The decision json is not an object."
        );
        return errors;
    }

    for (auto& [key, _] : decision_json.items()) {
        errors += parse_optional_attribute(decision_json, key.c_str(), decision_data.selections[key]);
    }

    return errors;
}
