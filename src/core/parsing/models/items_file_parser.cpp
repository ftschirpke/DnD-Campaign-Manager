#include "dnd_config.hpp"

#include "items_file_parser.hpp"

#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <nlohmann/json.hpp>

#include "core/controllers/groups.hpp"
#include "core/models/item.hpp"
#include "core/parsing/content_file_parser.hpp"
#include "core/parsing/parse_optionals.hpp"
#include "core/parsing/parsing_exceptions.hpp"
#include "core/parsing/parsing_types.hpp"

void dnd::ItemsFileParser::createItem(std::string_view item_name, const nlohmann::json& item_json) {
    ItemParsingInfo info;
    info.name = item_name;
    info.requires_attunement = item_json.at("requires_attunement").get<bool>();
    info.description = item_json.at("description").get<std::string>();
    if (item_json.contains("cosmetic_description")) {
        info.cosmetic_description = item_json.at("cosmetic_description").get<std::string>();
    } else {
        info.cosmetic_description = "";
    }
    item_parsing_info.emplace_back(std::move(info));
}

void dnd::ItemsFileParser::parse() {
    DND_MEASURE_FUNCTION();
    if (!json_to_parse.is_object()) {
        throw json_format_error(type, filepath, "map/object");
    }
    items_in_file = json_to_parse.size();
    for (const auto& [item_name, item_json] : json_to_parse.items()) {
        if (item_name.empty()) {
            throw invalid_attribute(type, filepath, "item name", "cannot be \"\".");
        }
        createItem(item_name, item_json);
    }
}

bool dnd::ItemsFileParser::validate() const {
    valid.reserve(items_in_file);
    for (const ItemParsingInfo& info : item_parsing_info) {
        if (items.contains(info.name)) {
            std::cerr << "Warning: Duplicate of item \"" << info.name << "\" found.\n";
            valid.emplace_back(false);
            continue;
        }
        valid.emplace_back(true);
    }
    return std::any_of(valid.cbegin(), valid.cend(), [](bool b) { return b; });
}

void dnd::ItemsFileParser::saveResult() {
    for (size_t i = 0; i < items_in_file; ++i) {
        if (valid[i]) {
            ItemParsingInfo& info = item_parsing_info[i];

            groups.add("items", info.name);
            if (info.requires_attunement) {
                groups.add("items with attunement", info.name);
            } else {
                groups.add("items without attunement", info.name);
            }

            items.create(info.name, filepath, info.requires_attunement, info.description, info.cosmetic_description);
        }
    }
}
