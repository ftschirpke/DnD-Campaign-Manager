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

std::optional<Error> check_required_attribute(
    const nlohmann::json& json, const char* attribute_name, const std::filesystem::path& filepath
) {
    if (!json.contains(attribute_name)) {
        return ParsingError(
            ParsingError::Code::MISSING_ATTRIBUTE, filepath,
            fmt::format("The attribute '{}' is missing", attribute_name)
        );
    }
    return std::nullopt;
}

std::optional<Error> check_required_index(
    const nlohmann::json& json, size_t index, const std::filesystem::path& filepath
) {
    if (json.size() <= index) {
        return ParsingError(
            ParsingError::Code::MISSING_ATTRIBUTE, filepath, fmt::format("Array does not have index {}", index)
        );
    }
    return std::nullopt;
}

} // namespace dnd
