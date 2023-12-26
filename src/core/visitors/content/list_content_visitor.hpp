#ifndef LIST_CONTENT_VISITOR_HPP_
#define LIST_CONTENT_VISITOR_HPP_

#include <dnd_config.hpp>

#include <string>
#include <utility>
#include <vector>

#include <core/models/character/character.hpp>
#include <core/models/character_class/character_class.hpp>
#include <core/models/character_species/character_species.hpp>
#include <core/models/character_subclass/character_subclass.hpp>
#include <core/models/character_subspecies/character_subspecies.hpp>
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
    virtual void operator()(const Character& character) override;
    virtual void operator()(const CharacterClass& character_class) override;
    virtual void operator()(const CharacterSubclass& character_subclass) override;
    virtual void operator()(const CharacterSpecies& character_species) override;
    virtual void operator()(const CharacterSubspecies& character_subspecies) override;
    virtual void operator()(const Item& item) override;
    virtual void operator()(const Spell& spell) override;
    virtual void operator()(const Feature& feature) override;
    virtual void operator()(const Choosable& choosable) override;
private:
    std::vector<std::string> string_list;
};


} // namespace dnd

#endif // LIST_CONTENT_VISITOR_HPP_
