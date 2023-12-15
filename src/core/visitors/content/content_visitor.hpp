#ifndef CONTENT_VISITOR_HPP_
#define CONTENT_VISITOR_HPP_

#include <dnd_config.hpp>

namespace dnd {

class Character;
class CharacterClass;
class CharacterSubclass;
class CharacterRace;
class CharacterSubrace;
class Item;
class Spell;
class Feature;
class Choosable;

class ContentVisitor {
public:
    virtual ~ContentVisitor() = default;
    virtual void operator()(const Character& character) = 0;
    virtual void operator()(const CharacterClass& character_class) = 0;
    virtual void operator()(const CharacterSubclass& character_subclass) = 0;
    virtual void operator()(const CharacterRace& character_race) = 0;
    virtual void operator()(const CharacterSubrace& character_subrace) = 0;
    virtual void operator()(const Item& item) = 0;
    virtual void operator()(const Spell& spell) = 0;
    virtual void operator()(const Feature& feature) = 0;
    virtual void operator()(const Choosable& choosable) = 0;
};

} // namespace dnd

#endif // CONTENT_VISITOR_HPP_
