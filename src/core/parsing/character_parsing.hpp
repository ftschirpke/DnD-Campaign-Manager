#ifndef CHARACTER_PARSING_HPP_
#define CHARACTER_PARSING_HPP_

#include <dnd_config.hpp>

#include <filesystem>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/models/character/character.hpp>

namespace dnd {

WithErrors<Character::Data> parse_character(const nlohmann::json& obj, const std::filesystem::path& filepath);

} // namespace dnd

#endif // CHARACTER_PARSING_HPP_
