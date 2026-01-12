#ifndef CHOICE_PARSING_HPP_
#define CHOICE_PARSING_HPP_

#include <dnd_config.hpp>

#include <expected>
#include <filesystem>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>

namespace dnd {

struct ParsedChoice {
    std::vector<std::string> options;
    int amount;
};

std::expected<ParsedChoice, Error> parse_choice(const nlohmann::json& obj, const std::filesystem::path& filepath);

} // namespace dnd

#endif // CHOICE_PARSING_HPP_
