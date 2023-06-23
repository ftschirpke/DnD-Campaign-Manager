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
class ChoosableFeature;

class ContentVisitor {
public:
    virtual ~ContentVisitor() = default;
    virtual void visit(const Character* character_ptr) = 0;
    virtual void visit(const CharacterClass* character_class_ptr) = 0;
    virtual void visit(const CharacterSubclass* character_subclass_ptr) = 0;
    virtual void visit(const CharacterRace* character_race_ptr) = 0;
    virtual void visit(const CharacterSubrace* character_subrace_ptr) = 0;
    virtual void visit(const Item* item_ptr) = 0;
    virtual void visit(const Spell* spell_ptr) = 0;
    virtual void visit(const Feature* feature_ptr) = 0;
    virtual void visit(const ChoosableFeature* choosable_ptr) = 0;
};

} // namespace dnd

#endif // CONTENT_VISITOR_HPP_
