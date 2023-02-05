#ifndef PARSING_TYPES_HPP_
#define PARSING_TYPES_HPP_

#include "dnd_config.hpp"

#include <string>
#include <unordered_map>

namespace dnd {

/**
 * @brief An enum for the types of contents that can be parsed
 */
enum ParsingType {
    GROUP,
    SPELL,
    RACE,
    CLASS,
    SUBRACE,
    SUBCLASS,
    CHARACTER,
};

// the name of the parsing types as strings
const std::unordered_map<ParsingType, std::string> parsing_type_names = {
    {ParsingType::GROUP, "Group"},         {ParsingType::SPELL, "Spell"},     {ParsingType::RACE, "Race"},
    {ParsingType::CLASS, "Class"},         {ParsingType::SUBRACE, "Subrace"}, {ParsingType::SUBCLASS, "Subclass"},
    {ParsingType::CHARACTER, "Character"},
};


} // namespace dnd

#endif // PARSING_TYPES_HPP_
