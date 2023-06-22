#include <dnd_config.hpp>

#include "effect_holder_parser.hpp"

#include <filesystem>
#include <iterator>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/parsing/parser.hpp>
#include <core/validation/effect_holder/choice/choice_data.hpp>
#include <core/validation/effect_holder/condition/condition_data.hpp>
#include <core/validation/effect_holder/effect/effect_data.hpp>
#include <core/validation/effect_holder/effect_holder_data.hpp>
#include <core/validation/effect_holder/subholders/action_holder_data.hpp>
#include <core/validation/effect_holder/subholders/extra_spells_holder_data.hpp>
#include <core/validation/effect_holder/subholders/proficiency_holder_data.hpp>
#include <core/validation/effect_holder/subholders/riv_holder_data.hpp>

dnd::EffectHolderParser::EffectHolderParser(const std::filesystem::path& filepath) noexcept : Parser(filepath) {}

dnd::Errors dnd::EffectHolderParser::parse(nlohmann::ordered_json&& json, dnd::EffectHolderData& data) const {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(
            ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(), "The effect holder json is not an object."
        );
        return errors;
    }
    errors += parse_activation_conditions(json, data.activation_conditions_data, data.get_parent());
    if (json.contains("choose")) {
        errors += parse_choices(json, data.choices_data, data.get_parent());
    }
    errors += parse_effects(json, data.effects_data, data.get_parent());
    errors += parse_action_holder(json, data.action_holder_data);
    errors += parse_extra_spells_holder(json, data.extra_spells_holder_data);
    errors += parse_proficiency_holder(json, data.proficiency_holder_data);
    errors += parse_riv_holder(json, data.riv_holder_data);
    return errors;
}

dnd::Errors dnd::EffectHolderParser::parse_activation_conditions(
    nlohmann::ordered_json& json, std::vector<dnd::ConditionData>& data, const dnd::ValidationData* parent
) const {
    Errors errors;
    bool has_activation = json.contains("activation");
    bool has_activations = json.contains("activations");
    if (has_activation && has_activations) {
        errors.add_parsing_error(
            ParsingErrorCode::UNEXPECTED_ATTRIBUTE, get_filepath(),
            "The effect holder json contains both \"activation\" and \"activations\"."
        );
    } else if (has_activation) {
        ConditionData& condition_data = data.emplace_back(parent);
        errors += parse_optional_attribute(json, "activation", condition_data.condition_str);
    } else if (has_activations) {
        std::vector<std::string> activation_conditions_strs;
        errors += parse_optional_attribute(json, "activations", activation_conditions_strs);
        for (const auto& activation_condition_str : activation_conditions_strs) {
            ConditionData& condition_data = data.emplace_back(parent);
            condition_data.condition_str = activation_condition_str;
        }
    }
    return errors;
}

dnd::Errors dnd::EffectHolderParser::parse_choices(
    nlohmann::ordered_json& json, std::vector<dnd::ChoiceData>& data, const dnd::ValidationData* parent
) const {
    Errors errors;
    if (!json["choose"].is_object()) {
        errors.add_parsing_error(
            ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(), "The 'choose' json is not an object."
        );
        return errors;
    }
    for (const auto& [attribute_name, choice_json] : json["choices"].items()) {
        Errors choice_errors;
        ChoiceData& choice_data = data.emplace_back(parent);
        choice_data.attribute_name = attribute_name;
        choice_errors += parse_required_attribute(choice_json, "amount", choice_data.amount);

        bool has_group = choice_json.contains("group");
        bool has_groups = choice_json.contains("groups");
        if (has_group && has_groups) {
            choice_errors.add_parsing_error(
                ParsingErrorCode::UNEXPECTED_ATTRIBUTE, get_filepath(),
                "The choice json contains both \"group\" and \"groups\"."
            );
        } else if (has_group) {
            std::string& group = choice_data.group_names.emplace_back();
            choice_errors += parse_optional_attribute(choice_json, "group", group);
        } else if (has_groups) {
            choice_errors += parse_optional_attribute(choice_json, "groups", choice_data.group_names);
        }

        choice_errors += parse_optional_attribute(choice_json, "choices", choice_data.explicit_choices);

        if (choice_errors.ok()) {
            data.emplace_back(choice_data);
        }
        errors += std::move(choice_errors);
    }
    return errors;
}

dnd::Errors dnd::EffectHolderParser::parse_effects(
    nlohmann::ordered_json& json, std::vector<dnd::EffectData>& data, const dnd::ValidationData* parent
) const {
    std::vector<std::string> effect_strings;
    Errors errors = parse_optional_attribute(json, "effects", effect_strings);
    for (auto& effect_string : effect_strings) {
        EffectData& effect_data = data.emplace_back(parent);
        effect_data.effect_str = std::move(effect_string);
    }
    return errors;
}

dnd::Errors dnd::EffectHolderParser::parse_action_holder(nlohmann::ordered_json& json, dnd::ActionHolderData& data)
    const {
    Errors errors;
    errors += parse_optional_attribute(json, "actions", data.actions);
    errors += parse_optional_attribute(json, "bonus_actions", data.bonus_actions);
    errors += parse_optional_attribute(json, "reactions", data.reactions);
    return errors;
}

dnd::Errors dnd::EffectHolderParser::parse_extra_spells_holder(
    nlohmann::ordered_json& json, dnd::ExtraSpellsHolderData& data
) const {
    Errors errors;
    errors += parse_optional_attribute(json, "cantrips_free", data.free_cantrips);
    errors += parse_optional_attribute(json, "spells_at_will", data.at_will);
    errors += parse_optional_attribute(json, "spells_innate", data.innate);
    errors += parse_optional_attribute(json, "spells_free_once_a_day", data.free_once_a_day);
    errors += parse_optional_attribute(json, "spells_known", data.spells_known);
    errors += parse_optional_attribute(json, "spells_known_included", data.spells_known_included);
    errors += parse_optional_attribute(json, "spells_added_to_spell_list", data.added_to_spell_list);

    std::vector<std::string> always_prepared;
    errors += parse_optional_attribute(json, "spells_always_prepared", always_prepared);
    data.spells_known.insert(
        std::make_move_iterator(always_prepared.begin()), std::make_move_iterator(always_prepared.end())
    );

    return errors;
}

dnd::Errors dnd::EffectHolderParser::parse_proficiency_holder(
    nlohmann::ordered_json& json, dnd::ProficiencyHolderData& data
) const {
    Errors errors;
    errors += parse_optional_attribute(json, "armor_proficiencies", data.armor);
    errors += parse_optional_attribute(json, "weapon_proficiencies", data.weapons);
    errors += parse_optional_attribute(json, "tool_proficiencies", data.tools);
    errors += parse_optional_attribute(json, "skill_proficiencies", data.skills);
    errors += parse_optional_attribute(json, "saving_throw_proficiencies", data.saving_throws);
    errors += parse_optional_attribute(json, "languages", data.languages);
    errors += parse_optional_attribute(json, "senses", data.senses);
    return errors;
}

dnd::Errors dnd::EffectHolderParser::parse_riv_holder(nlohmann::ordered_json& json, dnd::RIVHolderData& data) const {
    Errors errors;
    errors += parse_optional_attribute(json, "damage_resistances", data.damage_resistances);
    errors += parse_optional_attribute(json, "damage_immunities", data.damage_immunities);
    errors += parse_optional_attribute(json, "damage_vulnerabilities", data.damage_vulnerabilities);
    errors += parse_optional_attribute(json, "condition_immunities", data.condition_immunities);
    return errors;
}
