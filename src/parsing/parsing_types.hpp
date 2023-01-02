#ifndef PARSING_TYPES_HPP_
#define PARSING_TYPES_HPP_

#include <string>
#include <unordered_map>

namespace dnd {

enum ParsingType {
    SPELL,
    RACE,
    CLASS,
    SUBRACE,
    SUBCLASS,
    CHARACTER,
};

const std::unordered_map<ParsingType, std::string> parsing_type_names = {
    {ParsingType::SPELL, "Spell"},     {ParsingType::RACE, "Race"},         {ParsingType::CLASS, "Class"},
    {ParsingType::SUBRACE, "Subrace"}, {ParsingType::SUBCLASS, "Subclass"}, {ParsingType::CHARACTER, "Character"},
};


} // namespace dnd

#endif // PARSING_TYPES_HPP_
