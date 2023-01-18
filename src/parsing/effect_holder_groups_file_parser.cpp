#include "dnd_config.hpp"

#include "effect_holder_groups_file_parser.hpp"

#include <algorithm>
#include <filesystem>

#include <nlohmann/json.hpp>

#include "controllers/groups.hpp"
#include "models/effect_holder/choosable.hpp"
#include "parsing/parsing_exceptions.hpp"
#include "parsing/parsing_types.hpp"

void dnd::EffectHolderGroupsFileParser::parse() {
    DND_MEASURE_FUNCTION();
    if (!json_to_parse.is_object()) {
        throw json_format_error(ParsingType::GROUP, filepath, "map/object");
    }
    group_name = filepath.stem().c_str();
    std::replace(group_name.begin(), group_name.end(), '_', ' ');

    choosables.reserve(json_to_parse.size());

    for (const auto& [choosable_name, choosable_json] : json_to_parse.items()) {
        choosables.emplace(choosable_name, createChoosable(choosable_name, choosable_json));
    }
}

dnd::Choosable dnd::EffectHolderGroupsFileParser::createChoosable(
    const std::string& choosable_name, const nlohmann::json& choosable_json
) const {
    const std::string description = choosable_json.at("description").get<std::string>();

    // TODO: change choosable constructor?
    Choosable choosable(choosable_name, description);

    choosable.main_part = std::move(createEffectHolder(choosable_json));
    if (choosable_json.contains("multi")) {
        if (!choosable_json.is_array()) {
            throw attribute_format_error(filepath, "multi", "array");
        }
        for (const auto& part_json : choosable_json) {
            choosable.parts.emplace_back(std::move(createEffectHolder(part_json)));
        }
    }

    // TODO: parse prerequisites

    return choosable;
}


bool dnd::EffectHolderGroupsFileParser::validate() const {
    if (group_name.empty()) {
        std::cerr << "Warning: " << filepath << " contains group " << group_name << " with invalid name."
                  << " Group names cannot be \"\".\n";
        return false;
    } else if (group_name[0] == ' ' || group_name[group_name.size() - 1] == ' ') {
        std::cerr << "Warning: " << filepath << " contains group " << group_name << " with invalid name."
                  << " Group names cannot start or end with an underscore or space.\n";
        return false;
    }
    return true;
}

void dnd::EffectHolderGroupsFileParser::saveResult() { results.add(group_name, std::move(choosables)); }
