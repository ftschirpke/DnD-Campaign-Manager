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

    for (const auto& [name, choosable_json] : json_to_parse.items()) {
        choosables.emplace(name, createChoosable(name, choosable_json));
    }
}

dnd::Choosable dnd::EffectHolderGroupsFileParser::createChoosable(
    const std::string& name, const nlohmann::json& choosable_json
) const {
    // TODO: change choosable constructor?
    Choosable choosable(std::move(createEffectHolder(name, choosable_json)));

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
