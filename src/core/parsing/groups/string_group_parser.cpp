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
            errors += parse_optional_attribute(json, key.c_str(), members[key]);
        } else if (value.is_object()) {
            errors += parse_subgroups(value, key);
        } else {
            errors.add_parsing_error(
                ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(),
                fmt::format("The group '{}' is neither array nor object.", key)
            );
        }

        for (auto& group_member : members[key]) {
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

dnd::Errors dnd::StringGroupParser::validate(const dnd::Content& content) const {
    DND_UNUSED(content);
    Errors errors;
    for (const auto& [group_name, values] : subgroups) {
        if (values.contains("")) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, nullptr,
                fmt::format("The group '{}' cannot have a subgroup \"\".", group_name)
            );
        }
    }
    for (const auto& [group_name, values] : members) {
        if (values.contains("")) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, nullptr,
                fmt::format("The group '{}' contains an empty string.", group_name)
            );
        }
    }
    return errors;
}

void dnd::StringGroupParser::save_result(dnd::Content& content) {
    for (auto& [group_name, values] : subgroups) {
        content.set_subgroups(group_name, std::move(values));
    }
    for (auto& [group_name, values] : members) {
        content.add_group_members(group_name, std::move(values));
    }
}

dnd::Errors dnd::StringGroupParser::parse_subgroups(nlohmann::ordered_json& sub_json, const std::string& parent) {
    Errors errors;
    for (auto& [key, value] : sub_json.items()) {
        subgroups[parent].insert(key);
        if (members.contains(key)) {
            errors.add_parsing_error(
                ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(),
                fmt::format("The subgroup '{}' is defined multiple times.", key)
            );
        }
        if (value.is_array()) {
            std::set<std::string> values;
            errors += parse_optional_attribute(sub_json, key.c_str(), values);
            if (key == "__no_subgroup__") {
                members[parent].insert(std::make_move_iterator(values.begin()), std::make_move_iterator(values.end()));
            } else {
                members[key].insert(std::make_move_iterator(values.begin()), std::make_move_iterator(values.end()));
            }
        } else if (value.is_object()) {
            errors += parse_subgroups(value, key);
        } else {
            errors.add_parsing_error(
                ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(),
                fmt::format("The subgroup '{}' is neither array nor object.", key)
            );
        }
    }
    return errors;
}
