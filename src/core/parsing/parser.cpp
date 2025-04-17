#include <dnd_config.hpp>

#include "parser.hpp"

#include <deque>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <core/utils/types.hpp>

namespace dnd {

const std::filesystem::path& Parser::get_filepath() const { return filepath; }

Parser::Parser(const std::filesystem::path& filepath) : filepath(filepath) {}

std::optional<Error> write_formatted_description_into(
    const nlohmann::json& json, std::string& out, const std::filesystem::path& filepath
) {
    std::vector<std::string> entries;

    if (!json.contains("entries")) {
        return ParsingError(
            ParsingError::Code::MISSING_ATTRIBUTE, filepath,
            "To create a description, the attribute 'entries' is required"
        );
    }

    std::deque<CRef<nlohmann::json>> stack;
    stack.push_back(json);

    while (!stack.empty()) {
        const nlohmann::json& obj = stack.back();
        stack.pop_back();
        assert(obj.is_object());

        if (obj.contains("entries") && obj["entries"].is_array()) {
            for (const nlohmann::json& entry : obj["entries"]) {
                if (entry.is_string()) {
                    entries.emplace_back(entry.get<std::string>());
                } else if (entry.is_object()) {
                    stack.push_back(entry);
                }
            }
        } else if (obj.contains("items") && obj["items"].is_array()) {
            // HACK: this is a hacky way to make lists visible for now
            // TODO: should be handled as special format to allow for better formatting in frontend
            for (const nlohmann::json& entry : obj["items"]) {
                if (entry.is_string()) {
                    entries.emplace_back(fmt::format("- {}", entry.get<std::string>()));
                } else if (entry.is_object()) {
                    stack.push_back(entry);
                }
            }
        }
    }

    out = fmt::format("{}", fmt::join(entries.begin(), entries.end(), "\n"));
    return std::nullopt;
}

static const char* json_attribute_type_name(JsonType typ) {
    switch (typ) {
        case JsonType::STRING:
            return "string";
        case JsonType::ARRAY:
            return "array";
        case JsonType::OBJECT:
            return "object";
        case JsonType::ANY:
            return "any";
        default:
            assert(false);
            return "";
    }
}

static std::optional<Error> check_type(
    const nlohmann::json& json, JsonType typ, const std::filesystem::path& filepath, std::string&& error_msg
) {
    bool is_required_type = false;
    switch (typ) {
        case JsonType::STRING:
            is_required_type = json.is_string();
            break;
        case JsonType::ARRAY:
            is_required_type = json.is_array();
            break;
        case JsonType::OBJECT:
            is_required_type = json.is_object();
            break;
        case JsonType::ANY:
            is_required_type = true;
            break;
        default:
            assert(false);
            break;
    }
    if (!is_required_type) {
        return ParsingError(ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath, std::move(error_msg));
    }
    return std::nullopt;
}

std::optional<Error> check_required_attribute(
    const nlohmann::json& json, const char* attribute_name, const std::filesystem::path& filepath, JsonType typ
) {
    if (!json.contains(attribute_name)) {
        return ParsingError(
            ParsingError::Code::MISSING_ATTRIBUTE, filepath,
            fmt::format("The attribute '{}' is missing", attribute_name)
        );
    }
    return check_type(
        json[attribute_name], typ, filepath,
        fmt::format(
            "The attribute '{}' exists but is of wrong type - should be {}", attribute_name,
            json_attribute_type_name(typ)
        )
    );
}

std::optional<Error> check_required_index(
    const nlohmann::json& json, size_t index, const std::filesystem::path& filepath, JsonType typ
) {
    if (json.size() <= index) {
        return ParsingError(
            ParsingError::Code::MISSING_ATTRIBUTE, filepath, fmt::format("Array does not have index {}", index)
        );
    }
    return check_type(
        json[index], typ, filepath,
        fmt::format("The index {} exists but is of wrong type - should be {}", index, json_attribute_type_name(typ))
    );
    return std::nullopt;
}

} // namespace dnd
