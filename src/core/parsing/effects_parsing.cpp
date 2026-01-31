#include <dnd_config.hpp>

#include "effects_parsing.hpp"

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/parsing/parser.hpp>
#include <log.hpp>

namespace dnd {

// TODO: parse parts of Effects::Data :
// std::vector<Condition::Data> activation_conditions_data;
// std::vector<Choice::Data> choices_data;
// std::vector<StatChange::Data> stat_changes_data;
// ActionHolder::Data action_holder_data;
// ExtraSpellsHolder::Data extra_spells_holder_data;
// ProficiencyHolder::Data proficiency_holder_data;
// RIVHolder::Data riv_holder_data;

WithErrors<Effects::Data> parse_effects(const nlohmann::json& obj, const std::filesystem::path& filepath) {
    WithErrors<Effects::Data> result{};
    Effects::Data& effects_data = result.value;
    Errors& errors = result.errors;

    for (const nlohmann::json& effect_json : obj) {
        // TODO: properly implement
        if (!effect_json.contains("transfer") || !effect_json["transfer"]) {
            continue;
        }
        if (effect_json.contains("changes")) {
            for (const nlohmann::json& change_json : effect_json["changes"]) {
                Errors suberrors;
                std::string key;
                std::string mode;
                std::string value_str;
                suberrors += parse_required_attribute_into(change_json, "key", key, filepath);
                suberrors += parse_required_attribute_into(change_json, "mode", mode, filepath);
                if (change_json["value"].is_string()) {
                    suberrors += parse_required_attribute_into(change_json, "value", value_str, filepath);
                    if (value_str[0] == '-' && mode == "ADD") {
                        mode = "SUB";
                    }
                    auto start = value_str.begin();
                    auto end = value_str.end();
                    while (start != end) {
                        char c = *start;
                        if (std::isspace(c) || c == '+' || c == '-' || c == '(') {
                            ++start;
                        } else {
                            break;
                        }
                    }
                    while (end != start) {
                        char c = *(end - 1);
                        if (std::isspace(c) || c == ')') {
                            --end;
                        } else {
                            break;
                        }
                    }
                    value_str = std::string(start, end);
                } else if (change_json["value"].is_number_integer()) {
                    int value;
                    suberrors += parse_required_attribute_into(change_json, "value", value, filepath);
                    value_str = std::to_string(value);
                } else {
                    LOGWARN("Value of unexpected type for stat change value: '{}'", change_json["value"].dump());
                }
                if (errors.ok()) {
                    effects_data.stat_changes_data.emplace_back(
                        StatChange::Data{.stat_change_str = fmt::format("{} NORMAL {} ({})", key, mode, value_str)}
                    );
                }
            }
        }
    }

    return result;
}

} // namespace dnd
