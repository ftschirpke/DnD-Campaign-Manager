#include <dnd_config.hpp>

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

#include <core/basic_mechanics/abilities.hpp>
#include <core/controllers/groups.hpp>
#include <core/models/effect_holder/action_holder.hpp>
#include <core/models/effect_holder/activation.hpp>
#include <core/models/effect_holder/choice.hpp>
#include <core/models/effect_holder/effect.hpp>
#include <core/models/effect_holder/effect_holder.hpp>
#include <core/models/effect_holder/effect_holder_with_choices.hpp>
#include <core/models/effect_holder/extra_spells_holder.hpp>
#include <core/models/effect_holder/proficiency_holder.hpp>
#include <core/models/effect_holder/riv_holder.hpp>
#include <core/parsing/parse_optionals.hpp>
#include <core/parsing/parsing_exceptions.hpp>
#include <core/parsing/parsing_types.hpp>
#include <core/parsing/subparser.hpp>

constexpr const char* dnd::EffectHolderParser::
    activation_regex_cstr = "[A-Z][_A-Z0-9]+ (==|!=|>=|<=|>|<) ([A-Z][_A-Z0-9]+|-?\\d+(\\.\\d\\d?)?|true|false)";

constexpr const char*
    dnd::EffectHolderParser::effect_regex_cstr = "[A-Z][_A-Z0-9]+ (earliest|early|normal|late|latest) (("
                                                 "(add|mult|div|set|max|min) -?\\d+(\\.\\d\\d?)?)"
                                                 "|("
                                                 "addOther|multOther|divOther|setOther|maxOther|minOther"
                                                 "|"
                                                 "addConst|multConst|divConst|setConst|maxConst|minConst"
                                                 ") [A-Z][_A-Z0-9]+)";

static void parse_action_optionals(const nlohmann::json& effect_holder_json, dnd::EffectHolder* const effect_holder) {
    dnd::parse_optional(effect_holder_json, "actions", effect_holder->actions.actions);
    dnd::parse_optional(effect_holder_json, "bonus_actions", effect_holder->actions.bonus_actions);
    dnd::parse_optional(effect_holder_json, "reactions", effect_holder->actions.reactions);
}

static void parse_extra_spell_optionals(
    const nlohmann::json& effect_holder_json, dnd::EffectHolder* const effect_holder
) {
    dnd::parse_optional(effect_holder_json, "cantrips_free", effect_holder->extra_spells.free_cantrips);
    dnd::parse_optional(effect_holder_json, "spells_at_will", effect_holder->extra_spells.at_will);
    dnd::parse_optional(effect_holder_json, "spells_innate", effect_holder->extra_spells.innate);
    dnd::parse_optional(effect_holder_json, "spells_free_once_a_day", effect_holder->extra_spells.free_once_a_day);
    dnd::parse_optional(effect_holder_json, "spells_known", effect_holder->extra_spells.spells_known);
    dnd::parse_optional(effect_holder_json, "spells_known_included", effect_holder->extra_spells.spells_known_included);
    dnd::parse_optional(
        effect_holder_json, "spells_added_to_spell_list", effect_holder->extra_spells.added_to_spell_list
    );

    std::unordered_set<std::string> always_prepared;
    dnd::parse_optional(effect_holder_json, "spells_always_prepared", always_prepared);
    effect_holder->extra_spells.spells_known.insert(
        std::make_move_iterator(always_prepared.begin()), std::make_move_iterator(always_prepared.begin())
    );
}

static void parse_riv_optionals(const nlohmann::json& effect_holder_json, dnd::EffectHolder* const effect_holder) {
    dnd::parse_optional(effect_holder_json, "damage_resistances", effect_holder->rivs.damage_resistances);
    dnd::parse_optional(effect_holder_json, "damage_immunities", effect_holder->rivs.damage_immunities);
    dnd::parse_optional(effect_holder_json, "damage_vulnerabilities", effect_holder->rivs.damage_vulnerabilities);
    dnd::parse_optional(effect_holder_json, "condition_immunities", effect_holder->rivs.condition_immunities);
}

static void parse_proficiency_optionals(
    const nlohmann::json& effect_holder_json, dnd::EffectHolder* const effect_holder
) {
    dnd::parse_optional(effect_holder_json, "armor_proficiencies", effect_holder->proficiencies.armor);
    dnd::parse_optional(effect_holder_json, "weapon_proficiencies", effect_holder->proficiencies.weapons);
    dnd::parse_optional(effect_holder_json, "tool_proficiencies", effect_holder->proficiencies.tools);
    dnd::parse_optional(effect_holder_json, "skill_proficiencies", effect_holder->proficiencies.skills);
    dnd::parse_optional(effect_holder_json, "savingthrow_proficiencies", effect_holder->proficiencies.saving_throws);
    dnd::parse_optional(effect_holder_json, "languages", effect_holder->proficiencies.languages);
    dnd::parse_optional(effect_holder_json, "senses", effect_holder->proficiencies.senses);
}

void dnd::EffectHolderParser::parse_effect_holder(
    const nlohmann::json& effect_holder_json, dnd::EffectHolder* const effect_holder
) const {
    parse_action_optionals(effect_holder_json, effect_holder);
    parse_extra_spell_optionals(effect_holder_json, effect_holder);
    parse_riv_optionals(effect_holder_json, effect_holder);
    parse_proficiency_optionals(effect_holder_json, effect_holder);

    if (effect_holder_json.contains("activation") && effect_holder_json.contains("activations")) {
        throw invalid_attribute(type, filepath, "activation/activations", "attributes are mutally exclusive.");
    } else if (effect_holder_json.contains("activation")) {
        const std::string activation_str = effect_holder_json.at("activation").get<std::string>();
        parse_and_add_activation(activation_str, effect_holder);
    } else if (effect_holder_json.contains("activations")) {
        if (!effect_holder_json.at("activations").is_array()) {
            throw attribute_format_error(type, filepath, "activations", "array");
        }
        const std::vector<std::string> activation_strs = effect_holder_json.at("activations")
                                                             .get<std::vector<std::string>>();
        for (const std::string& activation_str : activation_strs) {
            parse_and_add_activation(activation_str, effect_holder);
        }
    }

    if (effect_holder_json.contains("effects")) {
        if (!effect_holder_json.at("effects").is_array()) {
            throw attribute_format_error(type, filepath, "effects", "array");
        }
        for (const auto& effect_val : effect_holder_json.at("effects")) {
            const std::string effect_str = effect_val.get<std::string>();
            parse_and_add_effect(effect_str, effect_holder);
        }
    }
}

dnd::EffectHolder dnd::EffectHolderParser::create_effect_holder(const nlohmann::json& effect_holder_json) const {
    DND_MEASURE_FUNCTION();

    // TODO: change effect holder constructor?
    EffectHolder effect_holder;

    parse_effect_holder(effect_holder_json, &effect_holder);

    return effect_holder;
}

dnd::EffectHolderWithChoices dnd::EffectHolderParser::create_effect_holder_with_choices(
    const nlohmann::json& effect_holder_json
) const {
    DND_MEASURE_FUNCTION();

    // TODO: change effect holder constructor?
    EffectHolderWithChoices effect_holder;

    parse_effect_holder(effect_holder_json, &effect_holder);

    if (!effect_holder_json.at("choose").is_object()) {
        throw attribute_format_error(type, filepath, "choose", "map/object");
    }
    if (effect_holder_json.at("choose").empty()) {
        throw invalid_attribute(type, filepath, "choose", "cannot be empty");
    }
    for (const auto& [choice_key, choice_json] : effect_holder_json.at("choose").items()) {
        parse_and_add_choice(choice_key, choice_json, effect_holder);
    }

    return effect_holder;
}

void dnd::EffectHolderParser::parse_and_add_choice(
    const std::string& choice_key, const nlohmann::json& choice_json, EffectHolderWithChoices& effect_holder
) const {
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
            if (!groups.is_group(group_name)) {
                throw invalid_attribute(type, filepath, "choose:group", '\"' + group_name + "\" is not a group");
            }
        }

        bool all_are_choosable_groups = std::all_of(
            group_names.begin(), group_names.end(),
            [&](const std::string& group_name) { return groups.is_choosable_group(group_name); }
        );
        bool all_are_string_groups = std::all_of(
            group_names.begin(), group_names.end(),
            [&](const std::string& group_name) { return groups.is_string_group(group_name); }
        );

        if (all_are_choosable_groups) {
            std::vector<const std::unordered_map<std::string, Choosable>*> group_values;
            group_values.reserve(group_names.size());
            for (const auto& group_name : group_names) {
                group_values.emplace_back(&groups.get_choosable_group(group_name));
            }
            effect_holder.choices.emplace_back(
                std::make_unique<ChoosableGroupChoice>(amount, choice_key, std::move(group_values))
            );
        } else if (all_are_string_groups) {
            std::vector<const std::unordered_set<std::string>*> group_values;
            group_values.reserve(group_names.size());
            for (const auto& group_name : group_names) {
                group_values.emplace_back(&groups.get_string_group(group_name));
            }
            effect_holder.choices.emplace_back(
                std::make_unique<StringGroupChoice>(amount, choice_key, std::move(group_values))
            );
        } else {
            throw invalid_attribute(type, filepath, "choose:group/groups", "all groups must be of the same kind.");
        }
    }
}

std::unique_ptr<dnd::Effect> dnd::EffectHolderParser::create_effect(const std::string& effect_str) const {
    if (!std::regex_match(effect_str, effect_regex)) {
        throw attribute_type_error(type, filepath, "invalid effect format: \"" + effect_str + "\"");
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

    bool effect_time_found = false;
    EffectTime effect_time = EffectTime::NORMAL; // just setting an initial value
    for (const auto& [effect_time_name, effect_time_val] : effect_times_in_order) {
        if (effect_time_name == effect_time_str) {
            effect_time = effect_time_val;
            effect_time_found = true;
            break;
        }
    }
    if (!effect_time_found) {
        throw attribute_type_error(type, filepath, "effect type \"" + effect_time_str + "\" does not exist");
    }

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
            return std::make_unique<IntNumEffect>(
                affected_attribute, effect_type, effect_time, static_cast<int>(effect_value * 100)
            );
            // attributes are stored as integers * 100, see CharacterState
        }
    } else {
        size_t other_idx = effect_type.find("Other");
        size_t const_idx = effect_type.find("Const");
        if (other_idx != std::string::npos) {
            const std::string op_name = effect_type.substr(0, other_idx);
            return std::make_unique<OtherAttributeEffect>(affected_attribute, op_name, effect_time, last_part);
        } else if (const_idx != std::string::npos) {
            const std::string op_name = effect_type.substr(0, const_idx);
            return std::make_unique<ConstEffect>(affected_attribute, op_name, effect_time, last_part);
        }
    }
    throw invalid_attribute(type, filepath, "effect", "unkown effect type");
}

void dnd::EffectHolderParser::parse_and_add_effect(const std::string& effect_str, EffectHolder* const effect_holder)
    const {
    auto effect = create_effect(effect_str);
    if (is_ability(std::string_view(effect->affected_attribute.c_str(), 3))) {
        effect_holder->ability_score_effects[effect->time].emplace_back(std::move(effect));
    } else {
        effect_holder->normal_effects[effect->time].emplace_back(std::move(effect));
    }
}

void dnd::EffectHolderParser::parse_and_add_activation(
    const std::string& activation_str, EffectHolder* const effect_holder
) const {
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
        right_value = static_cast<int>(std::stof(last_part) * 100);
        // attributes are stored as integers * 100, see CharacterState
    }
    effect_holder->activations.emplace_back(std::make_unique<NumericActivation>(left_identifier, op_name, right_value));
}
