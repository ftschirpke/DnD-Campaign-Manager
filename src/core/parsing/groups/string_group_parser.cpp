#include <dnd_config.hpp>

#include "string_group_parser.hpp"

#include <iterator>
#include <set>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include <core/controllers/content_holder.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/errors/validation_error.hpp>

dnd::StringGroupParser::StringGroupParser(const std::filesystem::path& file_path) : FileParser(file_path) {}

dnd::Errors dnd::StringGroupParser::parse() {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(
            ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(), "The string group json file is not an object."
        );
        return errors;
    }

    for (auto& [key, value] : json.items()) {
        if (key == "__no_subgroup__") {
            errors.add_parsing_error(
                ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(),
                "The group '__no_subgroup__' is not allowed in the root of the string group file."
            );
        } else if (value.is_array()) {
            errors += parse_optional_attribute(json, key.c_str(), data[key]);
        } else if (value.is_object()) {
            errors += parse_subgroups(value, data[key]);
        } else {
            errors.add_parsing_error(
                ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(),
                fmt::format("The group '{}' is neither array nor object.", key)
            );
        }
        for (auto& group_member : data[key]) {
            if (group_member.empty()) {
                errors.add_parsing_error(
                    ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(),
                    fmt::format("The group '{}' contains an empty string.", key)
                );
            }
        }
    }

    return errors;
}

dnd::Errors dnd::StringGroupParser::validate(const dnd::ContentHolder& content) const {
    DND_UNUSED(content);
    Errors errors;
    for (auto& [group_name, values] : data) {
        if (values.contains("")) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, nullptr,
                fmt::format("The group '{}' contains an empty string.", group_name)
            );
        }
    }
    return errors;
}

void dnd::StringGroupParser::save_result(dnd::ContentHolder& content) {
    for (auto& [group_name, values] : data) {
        content.groups.add(group_name, std::move(values));
    }
}

dnd::Errors dnd::StringGroupParser::parse_subgroups(
    nlohmann::ordered_json& sub_json, std::set<std::string>& parent_group_values
) {
    Errors errors;
    for (auto& [key, value] : sub_json.items()) {
        if (data.contains(key)) {
            errors.add_parsing_error(
                ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(),
                fmt::format("The subgroup '{}' is defined multiple times.", key)
            );
        }
        if (value.is_array()) {
            std::set<std::string> values;
            errors += parse_optional_attribute(sub_json, key.c_str(), values);
            if (key == "__no_subgroup__") {
                parent_group_values.insert(
                    std::make_move_iterator(values.begin()), std::make_move_iterator(values.end())
                );
            } else {
                data[key].insert(std::make_move_iterator(values.begin()), std::make_move_iterator(values.end()));
            }
        } else if (value.is_object()) {
            errors += parse_subgroups(value, data[key]);
        } else {
            errors.add_parsing_error(
                ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(),
                fmt::format("The subgroup '{}' is neither array nor object.", key)
            );
        }
        for (auto& group_member : data[key]) {
            parent_group_values.insert(group_member);
        }
    }
    return errors;
}
