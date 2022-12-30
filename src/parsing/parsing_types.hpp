#ifndef PARSING_TYPES_HPP_
#define PARSING_TYPES_HPP_

#include <string>
#include <unordered_map>

namespace dnd {

enum ParsingType {
    CHARACTERS,
    RACES,
    SUBRACES,
    CLASSES,
    SUBCLASSES,
    SPELLS,
};

const std::unordered_map<ParsingType, std::string> parsing_type_names = {
    {ParsingType::CHARACTERS, "Character"}, {ParsingType::RACES, "Race"},          {ParsingType::SUBRACES, "Subrace"},
    {ParsingType::CLASSES, "Class"},        {ParsingType::SUBCLASSES, "Subclass"}, {ParsingType::SPELLS, "Spell"},
};


} // namespace dnd

#endif // PARSING_TYPES_HPP_
