#include "dnd_config.hpp"

#include "effects_holder_file_parser.hpp"

#include <memory>
#include <regex>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "basic_mechanics/abilities.hpp"
#include "models/creature_state.hpp"
#include "models/effects_holder/activation.hpp"
#include "models/effects_holder/effects_holder.hpp"
#include "parsing/content_file_parser.hpp"
#include "parsing/parsing_exceptions.hpp"
#include "parsing/parsing_types.hpp"

dnd::EffectsHolder dnd::EffectsHolderFileParser::createEffectsHolder(
    const std::string& name, const nlohmann::json& effect_holder_json
) const {
    DND_MEASURE_FUNCTION();
    if (!effect_holder_json.is_object()) {
        throw attribute_format_error(filepath, name, "map/object");
    }
    const std::string description = effect_holder_json.at("description").get<std::string>();

    // TODO: change effects holder constructor?
    EffectsHolder effects_holder(name, description);

    parseOptional(effect_holder_json, "actions", effects_holder.actions.actions);
    parseOptional(effect_holder_json, "bonus_actions", effects_holder.actions.bonus_actions);
    parseOptional(effect_holder_json, "reactions", effects_holder.actions.reactions);

    parseOptional(effect_holder_json, "cantrips_free", effects_holder.extra_spells.free_cantrips);
    parseOptional(effect_holder_json, "spells_innate", effects_holder.extra_spells.innate_spells);
    parseOptional(effect_holder_json, "spells_at_will", effects_holder.extra_spells.spells_at_will);
    parseOptional(effect_holder_json, "spells_added_to_spell_list", effects_holder.extra_spells.added_to_spell_list);

    parseOptional(effect_holder_json, "damage_resistances", effects_holder.rivs.damage_resistances);
    parseOptional(effect_holder_json, "damage_immunities", effects_holder.rivs.damage_immunities);
    parseOptional(effect_holder_json, "damage_vulnerabilities", effects_holder.rivs.damage_vulnerabilities);
    parseOptional(effect_holder_json, "condition_immunities", effects_holder.rivs.condition_immunities);

    parseOptional(effect_holder_json, "armor_proficiencies", effects_holder.proficiencies.armor);
    parseOptional(effect_holder_json, "weapon_proficiencies", effects_holder.proficiencies.weapons);
    parseOptional(effect_holder_json, "tool_proficiencies", effects_holder.proficiencies.tools);
    parseOptional(effect_holder_json, "skill_proficiencies", effects_holder.proficiencies.skills);
    parseOptional(effect_holder_json, "savingthrow_proficiencies", effects_holder.proficiencies.saving_throws);
    parseOptional(effect_holder_json, "languages", effects_holder.proficiencies.languages);
    parseOptional(effect_holder_json, "senses", effects_holder.proficiencies.senses);

    if (effect_holder_json.contains("activation") && effect_holder_json.contains("activations")) {
        throw invalid_attribute(filepath, name + ":activation/activations", "attributes are mutally exclusive.");
    } else if (effect_holder_json.contains("activation")) {
        const std::string activation_str = effect_holder_json.at("activation").get<std::string>();
        parseAndAddActivation(activation_str, effects_holder);
    } else if (effect_holder_json.contains("activations")) {
        if (!effect_holder_json.at("activations").is_array()) {
            throw attribute_format_error(filepath, name + ":activations", "array");
        }
        const std::vector<std::string> activation_strs =
            effect_holder_json.at("activations").get<std::vector<std::string>>();
        for (const std::string& activation_str : activation_strs) {
            parseAndAddActivation(activation_str, effects_holder);
        }
    }

    if (effect_holder_json.contains("effects")) {
        if (!effect_holder_json.at("effects").is_array()) {
            throw attribute_format_error(filepath, name + ":effects", "array");
        }
        for (const auto& effect_val : effect_holder_json.at("effects")) {
            const std::string effect_str = effect_val.get<std::string>();
            parseAndAddEffect(effect_str, effects_holder);
        }
    }
    return effects_holder;
}

void dnd::EffectsHolderFileParser::parseAndAddEffect(const std::string& effect_str, EffectsHolder& effects_holder)
    const {
    if (!std::regex_match(effect_str, effect_regex)) {
        throw attribute_type_error(
            filepath, "effect holder \"" + effects_holder.name + "\": invalid effect format: \"" + effect_str + "\""
        );
    }
    std::string::const_iterator it = effect_str.cbegin();
    while (*it != ' ') {
        ++it;
    }
    const std::string affected_attribute(effect_str.cbegin(), it);
    ++it;
    std::string::const_iterator start_it = it;
    while (*it != ' ') {
        ++it;
    }
    const std::string effect_time_str(start_it, it);
    start_it = ++it;
    while (*it != ' ') {
        ++it;
    }
    const std::string effect_type(start_it, it);

    const std::string last_part = std::string(++it, effect_str.cend());
    std::unique_ptr<Effect> effect_ptr;
    if (effect_type.size() < 5) {
        const float effect_value = std::stof(last_part);
        if (effect_type == "mult" || effect_type == "div") {
            effect_ptr = std::make_unique<FloatNumEffect>(affected_attribute, effect_type, effect_value);
        } else {
            effect_ptr = std::make_unique<IntNumEffect>(affected_attribute, effect_type, effect_value * 100);
            // attributes are stored as integers * 100, see CreatureState
        }
    } else {
        int other_idx = effect_type.find("Other");
        int const_idx = effect_type.find("Const");
        if (other_idx != std::string::npos) {
            const std::string op_name(effect_type.cbegin(), effect_type.cbegin() + other_idx);
            effect_ptr = std::make_unique<OtherAttributeEffect>(affected_attribute, op_name, last_part);
        } else if (const_idx != std::string::npos) {
            const std::string op_name(effect_type.cbegin(), effect_type.cbegin() + const_idx);
            effect_ptr = std::make_unique<ConstEffect>(affected_attribute, op_name, last_part);
        }
    }

    const EffectTime effect_time = effect_time_for_string.at(effect_time_str);
    if (isAbility(affected_attribute)) {
        effects_holder.ability_score_effects[effect_time].emplace_back(std::move(effect_ptr));
    } else {
        effects_holder.normal_effects[effect_time].emplace_back(std::move(effect_ptr));
    }
}

void dnd::EffectsHolderFileParser::parseAndAddActivation(
    const std::string& activation_str, EffectsHolder& effects_holder
) const {
    if (!std::regex_match(activation_str, activation_regex)) {
        throw attribute_type_error(
            filepath,
            "effect holder \"" + effects_holder.name + "\": invalid activation format: \"" + activation_str + "\""
        );
    }
    std::string::const_iterator it = activation_str.cbegin();
    while (*it != ' ') {
        ++it;
    }
    const std::string left_identifier(activation_str.cbegin(), it);
    ++it;
    const std::string::const_iterator last_it = it;
    while (*it != ' ') {
        ++it;
    }
    const std::string op_name(last_it, it);
    ++it;
    const std::string last_part(it, activation_str.cend());

    if (last_part[0] >= 'A' && last_part[0] <= 'Z') {
        effects_holder.activations.emplace_back(
            std::make_unique<IdentifierActivation>(left_identifier, op_name, last_part)
        );
        return;
    }

    int right_value;
    if (last_part == "true") {
        right_value = true;
    } else if (last_part == "false") {
        right_value = false;
    } else {
        right_value = std::stof(last_part) * 100;
        // attributes are stored as integers * 100, see CreatureState
    }
    effects_holder.activations.emplace_back(std::make_unique<NumericActivation>(left_identifier, op_name, right_value));
}
