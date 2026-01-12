#include <dnd_config.hpp>

#include "choice_parsing.hpp"

#include <cassert>
#include <expected>
#include <filesystem>
#include <optional>
#include <string>

#include <fmt/format.h>
#include <fmt/ranges.h>
#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/parsing/parser.hpp>

namespace dnd {

std::expected<ParsedChoice, Error> parse_choice(const nlohmann::json& obj, const std::filesystem::path& filepath) {
    ParsedChoice choice = {.options = {}, .amount = 1};
    std::optional<Error> error;

    assert(obj.is_object());

    error = check_required_attribute(obj, "from", filepath, JsonType::ARRAY);
    if (error.has_value()) {
        return std::unexpected(error.value());
    }
    error = parse_required_attribute_into(obj, "from", choice.options, filepath);
    if (error.has_value()) {
        return std::unexpected(error.value());
    }

    error = parse_optional_attribute_into(obj, "amount", choice.amount, filepath);
    if (error.has_value()) {
        return std::unexpected(error.value());
    }

    return choice;
}

} // namespace dnd
