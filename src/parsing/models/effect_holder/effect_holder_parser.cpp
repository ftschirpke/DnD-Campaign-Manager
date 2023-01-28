#include "dnd_config.hpp"

#include "effect_holder_parser.hpp"

#include <algorithm>
#include <memory>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include "basic_mechanics/abilities.hpp"
#include "controllers/groups.hpp"
#include "models/effect_holder/action_holder.hpp"
#include "models/effect_holder/activation.hpp"
#include "models/effect_holder/choice.hpp"
#include "models/effect_holder/effect.hpp"
#include "models/effect_holder/effect_holder.hpp"
#include "models/effect_holder/effect_holder_with_choices.hpp"
#include "models/effect_holder/extra_spells_holder.hpp"
#include "models/effect_holder/proficiency_holder.hpp"
#include "models/effect_holder/riv_holder.hpp"
#include "parsing/parse_optionals.hpp"
#include "parsing/parsing_exceptions.hpp"
#include "parsing/parsing_types.hpp"
#include "parsing/subparser.hpp"

void parseActionsOptionals(const nlohmann::json& effect_holder_json, dnd::EffectHolder* const effect_holder) {
    dnd::parseOptional(effect_holder_json, "actions", effect_holder->actions.actions);
    dnd::parseOptional(effect_holder_json, "bonus_actions", effect_holder->actions.bonus_actions);
    dnd::parseOptional(effect_holder_json, "reactions", effect_holder->actions.reactions);
}

void parseExtraSpellsOptionals(const nlohmann::json& effect_holder_json, dnd::EffectHolder* const effect_holder) {
    dnd::parseOptional(effect_holder_json, "cantrips_free", effect_holder->extra_spells.free_cantrips);
    dnd::parseOptional(effect_holder_json, "spells_at_will", effect_holder->extra_spells.at_will);
    dnd::parseOptional(effect_holder_json, "spells_innate", effect_holder->extra_spells.innate);
    dnd::parseOptional(effect_holder_json, "spells_free_once_a_day", effect_holder->extra_spells.free_once_a_day);
    dnd::parseOptional(effect_holder_json, "spells_known", effect_holder->extra_spells.spells_known);
    dnd::parseOptional(effect_holder_json, "spells_known_included", effect_holder->extra_spells.spells_known_included);
    dnd::parseOptional(
        effect_holder_json, "spells_added_to_spell_list", effect_holder->extra_spells.added_to_spell_list
    );

    std::unordered_set<std::string> always_prepared;
    dnd::parseOptional(effect_holder_json, "spells_always_prepared", always_prepared);
    effect_holder->extra_spells.spells_known.insert(
        std::make_move_iterator(always_prepared.begin()), std::make_move_iterator(always_prepared.begin())
    );
}

void parseRIVsOptionals(const nlohmann::json& effect_holder_json, dnd::EffectHolder* const effect_holder) {
    dnd::parseOptional(effect_holder_json, "damage_resistances", effect_holder->rivs.damage_resistances);
    dnd::parseOptional(effect_holder_json, "damage_immunities", effect_holder->rivs.damage_immunities);
    dnd::parseOptional(effect_holder_json, "damage_vulnerabilities", effect_holder->rivs.damage_vulnerabilities);
    dnd::parseOptional(effect_holder_json, "condition_immunities", effect_holder->rivs.condition_immunities);
}

void parseProficienciesOptionals(const nlohmann::json& effect_holder_json, dnd::EffectHolder* const effect_holder) {
    dnd::parseOptional(effect_holder_json, "armor_proficiencies", effect_holder->proficiencies.armor);
    dnd::parseOptional(effect_holder_json, "weapon_proficiencies", effect_holder->proficiencies.weapons);
    dnd::parseOptional(effect_holder_json, "tool_proficiencies", effect_holder->proficiencies.tools);
    dnd::parseOptional(effect_holder_json, "skill_proficiencies", effect_holder->proficiencies.skills);
    dnd::parseOptional(effect_holder_json, "savingthrow_proficiencies", effect_holder->proficiencies.saving_throws);
    dnd::parseOptional(effect_holder_json, "languages", effect_holder->proficiencies.languages);
    dnd::parseOptional(effect_holder_json, "senses", effect_holder->proficiencies.senses);
}

void dnd::EffectHolderParser::parseEffectHolder(
    const nlohmann::json& effect_holder_json, dnd::EffectHolder* const effect_holder
) const {
    requiresConfiguration();

    parseActionsOptionals(effect_holder_json, effect_holder);
    parseExtraSpellsOptionals(effect_holder_json, effect_holder);
    parseRIVsOptionals(effect_holder_json, effect_holder);
    parseProficienciesOptionals(effect_holder_json, effect_holder);

    if (effect_holder_json.contains("activation") && effect_holder_json.contains("activations")) {
        throw invalid_attribute(type, filepath, "activation/activations", "attributes are mutally exclusive.");
    } else if (effect_holder_json.contains("activation")) {
        const std::string activation_str = effect_holder_json.at("activation").get<std::string>();
        parseAndAddActivation(activation_str, effect_holder);
    } else if (effect_holder_json.contains("activations")) {
        if (!effect_holder_json.at("activations").is_array()) {
            throw attribute_format_error(type, filepath, "activations", "array");
        }
        const std::vector<std::string> activation_strs = effect_holder_json.at("activations")
                                                             .get<std::vector<std::string>>();
        for (const std::string& activation_str : activation_strs) {
            parseAndAddActivation(activation_str, effect_holder);
        }
    }

    if (effect_holder_json.contains("effects")) {
        if (!effect_holder_json.at("effects").is_array()) {
            throw attribute_format_error(type, filepath, "effects", "array");
        }
        for (const auto& effect_val : effect_holder_json.at("effects")) {
            const std::string effect_str = effect_val.get<std::string>();
            parseAndAddEffect(effect_str, effect_holder);
        }
    }
}

dnd::EffectHolder dnd::EffectHolderParser::createEffectHolder(const nlohmann::json& effect_holder_json) const {
    DND_MEASURE_FUNCTION();

    // TODO: change effect holder constructor?
    EffectHolder effect_holder;

    parseEffectHolder(effect_holder_json, &effect_holder);

    return effect_holder;
}

dnd::EffectHolderWithChoices dnd::EffectHolderParser::createEffectHolderWithChoices(
    const nlohmann::json& effect_holder_json
) const {
    DND_MEASURE_FUNCTION();

    requiresConfiguration();

    // TODO: change effect holder constructor?
    EffectHolderWithChoices effect_holder;

    parseEffectHolder(effect_holder_json, &effect_holder);

    if (!effect_holder_json.at("choose").is_object()) {
        throw attribute_format_error(type, filepath, "choose", "map/object");
    }
    for (const auto& [choice_key, choice_json] : effect_holder_json.at("choose").items()) {
        parseAndAddChoice(choice_key, choice_json, effect_holder);
    }

    return effect_holder;
}

void dnd::EffectHolderParser::parseAndAddChoice(
    const std::string& choice_key, const nlohmann::json& choice_json, EffectHolderWithChoices& effect_holder
) const {
    requiresConfiguration();

    int amount = choice_json.at("amount").get<int>();

    if (choice_json.contains("choices")) {
        std::vector<std::string> selection = choice_json.at("choices").get<std::vector<std::string>>();

        effect_holder.choices.emplace_back(std::make_unique<SelectionChoice>(amount, choice_key, std::move(selection)));
    } else {
        std::vector<std::string> group_names;
        if (choice_json.contains("groups")) {
            group_names = choice_json.at("groups").get<std::vector<std::string>>();
        } else if (choice_json.contains("group")) {
            group_names.emplace_back(choice_json.at("group").get<std::string>());
        } else {
            group_names.emplace_back(choice_key);
        }

        for (auto& group_name : group_names) {
            size_t idx = group_name.find('_');
            while (idx != std::string::npos) {
                group_name[idx] = ' ';
                idx = group_name.find('_');
            }
            if (!groups.isGroup(group_name)) {
                throw invalid_attribute(type, filepath, "choose:group", '\"' + group_name + "\" is not a group");
            }
        }

        bool all_are_choosable_groups = std::all_of(
            group_names.begin(), group_names.end(),
            [&](const std::string& group_name) { return groups.isChoosableGroup(group_name); }
        );
        bool all_are_string_groups = std::all_of(
            group_names.begin(), group_names.end(),
            [&](const std::string& group_name) { return groups.isStringGroup(group_name); }
        );

        if (all_are_choosable_groups) {
            std::vector<const std::unordered_map<std::string, Choosable>*> group_values;
            group_values.reserve(group_names.size());
            for (const auto& group_name : group_names) {
                group_values.emplace_back(&groups.getChoosableGroup(group_name));
            }
            effect_holder.choices.emplace_back(
                std::make_unique<ChoosableGroupChoice>(amount, choice_key, std::move(group_values))
            );
        } else if (all_are_string_groups) {
            std::vector<const std::unordered_set<std::string>*> group_values;
            group_values.reserve(group_names.size());
            for (const auto& group_name : group_names) {
                group_values.emplace_back(&groups.getStringGroup(group_name));
            }
            effect_holder.choices.emplace_back(
                std::make_unique<StringGroupChoice>(amount, choice_key, std::move(group_values))
            );
        } else {
            throw invalid_attribute(type, filepath, "choose:group/groups", "all groups must be of the same kind.");
        }
    }
}

std::unique_ptr<dnd::Effect> dnd::EffectHolderParser::createEffect(const std::string& effect_str) const {
    requiresConfiguration();

    if (!std::regex_match(effect_str, effect_regex)) {
        throw attribute_type_error(type, filepath, "invalid effect format: \"" + effect_str + "\"");
    }
    auto it = effect_str.cbegin();
    while (*it != ' ') {
        ++it;
    }
    const std::string affected_attribute(effect_str.cbegin(), it);
    ++it;
    auto start_it = it;
    while (*it != ' ') {
        ++it;
    }

    const std::string effect_time_str(start_it, it);
    const EffectTime effect_time = effect_time_for_string.at(effect_time_str);

    start_it = ++it;
    while (*it != ' ') {
        ++it;
    }
    const std::string effect_type(start_it, it);

    const std::string last_part = std::string(++it, effect_str.cend());
    if (effect_type.size() < 5) {
        const float effect_value = std::stof(last_part);
        if (effect_type == "mult" || effect_type == "div") {
            return std::make_unique<FloatNumEffect>(affected_attribute, effect_type, effect_time, effect_value);
        } else {
            return std::make_unique<IntNumEffect>(affected_attribute, effect_type, effect_time, effect_value * 100);
            // attributes are stored as integers * 100, see CreatureState
        }
    } else {
        size_t other_idx = effect_type.find("Other");
        size_t const_idx = effect_type.find("Const");
        if (other_idx != std::string::npos) {
            const std::string op_name(effect_type.cbegin(), effect_type.cbegin() + other_idx);
            return std::make_unique<OtherAttributeEffect>(affected_attribute, op_name, effect_time, last_part);
        } else if (const_idx != std::string::npos) {
            const std::string op_name(effect_type.cbegin(), effect_type.cbegin() + const_idx);
            return std::make_unique<ConstEffect>(affected_attribute, op_name, effect_time, last_part);
        }
    }
    throw invalid_attribute(type, filepath, "effect", "unkown effect type");
}

void dnd::EffectHolderParser::parseAndAddEffect(const std::string& effect_str, EffectHolder* const effect_holder)
    const {
    auto effect = createEffect(effect_str);
    if (isAbility(effect->affected_attribute)) {
        effect_holder->ability_score_effects[effect->time].emplace_back(std::move(effect));
    } else {
        effect_holder->normal_effects[effect->time].emplace_back(std::move(effect));
    }
}

void dnd::EffectHolderParser::parseAndAddActivation(
    const std::string& activation_str, EffectHolder* const effect_holder
) const {
    requiresConfiguration();

    if (!std::regex_match(activation_str, activation_regex)) {
        throw attribute_type_error(type, filepath, "invalid activation format: \"" + activation_str + "\"");
    }
    auto it = activation_str.cbegin();
    while (*it != ' ') {
        ++it;
    }
    const std::string left_identifier(activation_str.cbegin(), it);
    ++it;
    const auto last_it = it;
    while (*it != ' ') {
        ++it;
    }
    const std::string op_name(last_it, it);
    ++it;
    const std::string last_part(it, activation_str.cend());

    if (last_part[0] >= 'A' && last_part[0] <= 'Z') {
        effect_holder->activations.emplace_back(
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
    effect_holder->activations.emplace_back(std::make_unique<NumericActivation>(left_identifier, op_name, right_value));
}
