#ifndef PARSING_TYPES_HPP_
#define PARSING_TYPES_HPP_

#include <dnd_config.hpp>

#include <array>
#include <stdexcept>
#include <utility>

namespace dnd {

/**
 * @brief An enum for the types of contents that can be parsed
 */
enum ParsingType {
    GROUP,
    ITEM,
    SPELL,
    RACE,
    CLASS,
    SUBRACE,
    SUBCLASS,
    CHARACTER,
};

// the parsing types paired with their c-style string names
inline constexpr std::array<std::pair<ParsingType, const char*>, 8> parsing_type_names = {
    std::pair(ParsingType::GROUP, "Group"),         std::pair(ParsingType::SPELL, "Spell"),
    std::pair(ParsingType::RACE, "Race"),           std::pair(ParsingType::CLASS, "Class"),
    std::pair(ParsingType::SUBRACE, "Subrace"),     std::pair(ParsingType::SUBCLASS, "Subclass"),
    std::pair(ParsingType::CHARACTER, "Character"), std::pair(ParsingType::ITEM, "Item"),
};

/**
 * @brief Returns the name of a parsing type
 * @param parsing_type the parsing type
 * @return the name of the parsing type
 * @throws std::out_of_range if the parsing type does not exist or no name is implemented for it
 */
constexpr const char* parsingTypeName(ParsingType parsing_type) {
    for (const auto& [parsing_type_val, parsing_type_name] : parsing_type_names) {
        if (parsing_type_val == parsing_type) {
            return parsing_type_name;
        }
    }
    throw std::out_of_range("No such parsing type exists.");
}

} // namespace dnd

#endif // PARSING_TYPES_HPP_
