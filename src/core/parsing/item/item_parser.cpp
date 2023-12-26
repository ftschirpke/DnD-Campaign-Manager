#include <dnd_config.hpp>

#include "item_parser.hpp"

#include <cassert>
#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/models/item/item.hpp>
#include <core/validation/item/item_data.hpp>

namespace dnd {

ItemParser::ItemParser(const std::filesystem::path& file_path) : FileParser(file_path, true) {}

Errors ItemParser::parse() {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(
            ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(), "The item file is not a json object"
        );
        return errors;
    }
    item_data.reserve(json.size());
    for (auto& [item_name, item_json] : json.items()) {
        if (!item_json.is_object()) {
            errors.add_parsing_error(
                ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(),
                fmt::format("The item '{}' is not a json object", item_name)
            );
            continue;
        }
        Errors item_errors;
        ItemData data;
        data.name = item_name;
        data.source_path = get_filepath();
        item_errors += parse_required_attribute_into(item_json, "description", data.description);
        item_errors += parse_required_attribute_into(item_json, "requires_attunement", data.requires_attunement);
        item_errors += parse_optional_attribute_into(item_json, "cosmetic_description", data.cosmetic_description);
        if (item_errors.ok()) {
            item_data.emplace_back(std::move(data));
        }
        errors += std::move(item_errors);
    }
    items_in_file = item_data.size();
    return errors;
}

Errors ItemParser::validate(const Content& content) const {
    Errors errors;
    assert(items_in_file == item_data.size());
    item_data_valid.resize(items_in_file, false);
    for (size_t i = 0; i < items_in_file; ++i) {
        Errors validation_errors = item_data[i].validate();
        validation_errors += item_data[i].validate_relations(content);
        item_data_valid[i] = validation_errors.ok();
        errors += std::move(validation_errors);
    }
    return errors;
}

void ItemParser::save_result(Content& content) {
    assert(items_in_file == item_data_valid.size());
    assert(items_in_file == item_data.size());
    for (size_t i = 0; i < items_in_file; ++i) {
        if (item_data_valid[i]) {
            content.add_item(Item::create(std::move(item_data[i])));
        }
    }
}

} // namespace dnd
