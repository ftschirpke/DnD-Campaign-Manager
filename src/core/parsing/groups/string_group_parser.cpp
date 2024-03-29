#include <dnd_config.hpp>

#include "string_group_parser.hpp"

#include <iterator>
#include <set>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/errors/validation_error.hpp>
#include <core/parsing/file_parser.hpp>

namespace dnd {

StringGroupParser::StringGroupParser(const std::filesystem::path& file_path) : FileParser(file_path, false) {}

Errors StringGroupParser::parse() {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(
            ParsingError::Code::INVALID_FILE_FORMAT, get_filepath(), "The string group json file is not an object."
        );
        return errors;
    }

    for (const auto& [key, value] : json.items()) {
        if (key == "__no_subgroup__") {
            errors.add_parsing_error(
                ParsingError::Code::INVALID_FILE_FORMAT, get_filepath(),
                "The group '__no_subgroup__' is not allowed in the root of the string group file."
            );
        } else if (value.is_array()) {
            errors += parse_optional_attribute_into(json, key.c_str(), members[key]);
        } else if (value.is_object()) {
            errors += parse_subgroups(value, key);
        } else {
            errors.add_parsing_error(
                ParsingError::Code::INVALID_FILE_FORMAT, get_filepath(),
                fmt::format("The group '{}' is neither array nor object.", key)
            );
        }

        for (const std::string& group_member : members[key]) {
            if (group_member.empty()) {
                errors.add_parsing_error(
                    ParsingError::Code::INVALID_FILE_FORMAT, get_filepath(),
                    fmt::format("The group '{}' contains an empty string.", key)
                );
            }
        }
    }

    return errors;
}

void StringGroupParser::save_result(Content& content) {
    for (auto& [group_name, values] : subgroups) {
        content.set_subgroups(group_name, std::move(values));
    }
    for (auto& [group_name, values] : members) {
        content.add_group_members(group_name, std::move(values));
    }
}

Errors StringGroupParser::parse_subgroups(nlohmann::ordered_json& sub_json, const std::string& parent) {
    Errors errors;
    for (auto& [key, value] : sub_json.items()) {
        subgroups[parent].insert(key);
        if (members.contains(key)) {
            errors.add_parsing_error(
                ParsingError::Code::INVALID_FILE_FORMAT, get_filepath(),
                fmt::format("The subgroup '{}' is defined multiple times.", key)
            );
        }
        if (value.is_array()) {
            std::set<std::string> values;
            errors += parse_optional_attribute_into(sub_json, key.c_str(), values);
            if (key == "__no_subgroup__") {
                members[parent].insert(std::make_move_iterator(values.begin()), std::make_move_iterator(values.end()));
            } else {
                members[key].insert(std::make_move_iterator(values.begin()), std::make_move_iterator(values.end()));
            }
        } else if (value.is_object()) {
            errors += parse_subgroups(value, key);
        } else {
            errors.add_parsing_error(
                ParsingError::Code::INVALID_FILE_FORMAT, get_filepath(),
                fmt::format("The subgroup '{}' is neither array nor object.", key)
            );
        }
    }
    return errors;
}

} // namespace dnd
