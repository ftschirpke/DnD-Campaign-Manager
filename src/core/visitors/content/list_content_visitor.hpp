#ifndef LIST_CONTENT_VISITOR_HPP_
#define LIST_CONTENT_VISITOR_HPP_

#include <dnd_config.hpp>

#include <string>
#include <utility>
#include <vector>

#include <core/models/character/character.hpp>
#include <core/models/character_class/character_class.hpp>
#include <core/models/character_race/character_race.hpp>
#include <core/models/character_subclass/character_subclass.hpp>
#include <core/models/character_subrace/character_subrace.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/spell/spell.hpp>
#include <core/visitors/content/content_visitor.hpp>

namespace dnd {

class ListContentVisitor : public ContentVisitor {
public:
    void reserve(size_t size);
    std::vector<std::string> get_list();
    virtual void visit(const Character& character) override;
    virtual void visit(const CharacterClass& character_class) override;
    virtual void visit(const CharacterSubclass& character_subclass) override;
    virtual void visit(const CharacterRace& character_race) override;
    virtual void visit(const CharacterSubrace& character_subrace) override;
    virtual void visit(const Item& item) override;
    virtual void visit(const Spell& spell) override;
    virtual void visit(const Feature& feature) override;
    virtual void visit(const Choosable& choosable) override;
private:
    std::vector<std::string> string_list;
};


} // namespace dnd

#endif // LIST_CONTENT_VISITOR_HPP_
