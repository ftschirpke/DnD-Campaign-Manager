#include <dnd_config.hpp>

#include "parser.hpp"

#include <filesystem>

namespace dnd {

const std::filesystem::path& Parser::get_filepath() const { return filepath; }

Parser::Parser(const std::filesystem::path& filepath) : filepath(filepath) {}

bool Parser::contains_required_attribute(const nlohmann::json& json, const char* attribute_name, Errors& errors) const {
    if (!json.contains(attribute_name)) {
        errors.add_parsing_error(
            ParsingError::Code::MISSING_ATTRIBUTE, filepath,
            fmt::format("The attribute '{}' is missing", attribute_name)
        );
        return false;
    }
    return true;
}

bool Parser::contains_required_index(const nlohmann::json& json, size_t index, Errors& errors) const {
    if (json.size() <= index) {
        errors.add_parsing_error(
            ParsingError::Code::MISSING_ATTRIBUTE, filepath, fmt::format("Array does not have index {}", index)
        );
        return false;
    }
    return true;
}

} // namespace dnd
