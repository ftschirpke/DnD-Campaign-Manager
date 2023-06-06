#include <dnd_config.hpp>

#include "string_groups_file_parser.hpp"

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include <core/controllers/groups.hpp>
#include <core/parsing/parsing_exceptions.hpp>
#include <core/parsing/parsing_types.hpp>

std::unordered_set<std::string> dnd::StringGroupsFileParser::parseMap(const nlohmann::json& json_map) {
    std::unordered_set<std::string> no_subgroup_values;
    for (const auto& [name, value] : json_map.items()) {
        if (value.is_array()) {
            if (name == "__no_subgroup__") {
                no_subgroup_values = value.get<std::unordered_set<std::string>>();
            } else {
                auto values = value.get<std::unordered_set<std::string>>();
                parsed_data[name].insert(values.cbegin(), values.cend());
            }
        } else if (value.is_object()) {
            std::vector<std::string> subgroups;
            for (const auto& [subgroup, _] : value.items()) {
                if (subgroup != "__no_subgroup__") {
                    subgroups.push_back(subgroup);
                }
            }
            std::unordered_set<std::string> no_subgroup = parseMap(value);
            parsed_data[name].insert(no_subgroup.cbegin(), no_subgroup.cend());
            for (const auto& subgroup : subgroups) {
                parsed_data[name].insert(parsed_data.at(subgroup).cbegin(), parsed_data.at(subgroup).cend());
            }
        } else {
            throw attribute_format_error(ParsingType::GROUP, filepath, name, "map/object or array");
        }
    }
    return no_subgroup_values;
}

void dnd::StringGroupsFileParser::parse() {
    DND_MEASURE_FUNCTION();
    if (!json_to_parse.is_object()) {
        throw json_format_error(ParsingType::GROUP, filepath, "map/object");
    }
    auto no_subgroup = parseMap(json_to_parse);
    if (!no_subgroup.empty()) {
        throw invalid_attribute(
            ParsingType::GROUP, filepath, "__no_subgroup__", "cannot exist at highest level of the JSON map/object."
        );
    }
}

bool dnd::StringGroupsFileParser::validate() const {
    bool valid = true;
    for (const auto& [group_name, _] : parsed_data) {
        if (group_name.empty()) {
            valid = false;
            std::cerr << "Warning: " << filepath << " contains group " << group_name << " with invalid name."
                      << " Group names cannot be \"\".\n";
        } else if (group_name[0] == '_' || group_name[group_name.size() - 1] == '_') {
            valid = false;
            std::cerr << "Warning: " << filepath << " contains group " << group_name << " with invalid name."
                      << " Group names cannot start or end with an underscore.\n";
        }
    }
    return valid;
}

void dnd::StringGroupsFileParser::save_result() {
    for (auto& [group_name, group_members] : parsed_data) {
        groups.add(std::move(group_name), std::move(group_members));
    }
}
