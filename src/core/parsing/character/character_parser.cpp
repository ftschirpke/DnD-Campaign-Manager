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
#include <core/models/effects_provider/feature.hpp>
#include <core/parsing/effects_provider/feature_parser.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/validation/character/character_data.hpp>
#include <core/validation/character/decision/decision_data.hpp>

dnd::CharacterParser::CharacterParser(const std::filesystem::path& filepath) noexcept
    : FileParser(filepath, false), feature_parser(filepath) {}

dnd::Errors dnd::CharacterParser::parse() {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(
            ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(), "The character json is not an object."
        );
    }

    errors += parse_required_attribute_into(json, "name", data.name);
    errors += parse_required_attribute_into(json, "description", data.description);
    data.source_path = get_filepath();

    errors += parse_required_attribute_into(json, "base_ability_scores", data.base_ability_scores_data.ability_scores);
    errors += parse_required_attribute_into(json, "race", data.character_basis_data.race_name);
    errors += parse_required_attribute_into(json, "class", data.character_basis_data.class_name);
    errors += parse_optional_attribute_into(json, "subrace", data.character_basis_data.subrace_name);
    errors += parse_optional_attribute_into(json, "subclass", data.character_basis_data.subclass_name);

    bool has_level = json.contains("level");
    bool has_xp = json.contains("xp");
    if (!has_level && !has_xp) {
        errors.add_parsing_error(ParsingErrorCode::MISSING_ATTRIBUTE, get_filepath(), "Character has no level or xp.");
    } else {
        errors += parse_optional_attribute_into(json, "level", data.progression_data.level);
        errors += parse_optional_attribute_into(json, "xp", data.progression_data.xp);
    }
    errors += parse_required_attribute_into(json, "hit_dice_rolls", data.progression_data.hit_dice_rolls);

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
        errors += feature_parser.parse_multiple_into(std::move(json["features"]), data.features_data, &data);
    }

    return errors;
}

static bool evaluate_effects_for_decision(
    const dnd::Effects& effects, dnd::DecisionData& decision_data, std::set<const dnd::Effects*>& processed_effects
) {
    if (!effects.get_choices().empty() && !processed_effects.contains(&effects)) {
        decision_data.set_target(&effects);
        processed_effects.emplace(&effects);
        return true;
    }
    return false;
}

void dnd::CharacterParser::set_context(const dnd::Content& content) {
    std::set<const Effects*> processed_effects;
    for (DecisionData& decision_data : data.decisions_data) {
        std::optional<EffectsProviderVariant> effects_provider_optional = content.get_effects_provider(
            decision_data.feature_name
        );
        if (!effects_provider_optional.has_value()) {
            continue;
        }
        EffectsProviderVariant effects_provider_variant = effects_provider_optional.value();
        switch (effects_provider_variant.index()) {
            case 0: {
                const Feature& feature = std::get<0>(effects_provider_variant);
                evaluate_effects_for_decision(feature.get_main_effects(), decision_data, processed_effects);
                break;
            }
            case 1: {
                const ClassFeature& class_feature = std::get<1>(effects_provider_variant);
                evaluate_effects_for_decision(class_feature.get_main_effects(), decision_data, processed_effects);
                for (const auto& [level, effects] : class_feature.get_higher_level_effects()) {
                    if (level > data.progression_data.level) {
                        break; // effects are not yet available and character cannot make decisions about them
                    }
                    evaluate_effects_for_decision(effects, decision_data, processed_effects);
                }
                break;
            }
            case 2: {
                const Choosable& choosable = std::get<2>(effects_provider_variant);
                evaluate_effects_for_decision(choosable.get_main_effects(), decision_data, processed_effects);
                break;
            }
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
        errors += parse_optional_attribute_into(decision_json, key.c_str(), decision_data.selections[key]);
    }

    return errors;
}
