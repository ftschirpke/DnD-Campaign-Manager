#ifndef DISPLAY_VISITOR_HPP_
#define DISPLAY_VISITOR_HPP_

#include <dnd_config.hpp>

#include <core/models/character/character.hpp>
#include <core/models/character_class/character_class.hpp>
#include <core/models/character_race/character_race.hpp>
#include <core/models/character_subclass/character_subclass.hpp>
#include <core/models/character_subrace/character_subrace.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/spell/spell.hpp>
#include <core/output/string_formatting/string_formatter.hpp>
#include <core/visitors/content/content_visitor.hpp>
#include <gui/string_formatting/display_format_visitor.hpp>

namespace dnd {

class DisplayVisitor : public ContentVisitor {
public:
    virtual void operator()(const Character& character) override;
    virtual void operator()(const CharacterClass& character_class) override;
    virtual void operator()(const CharacterSubclass& character_subclass) override;
    virtual void operator()(const CharacterRace& character_race) override;
    virtual void operator()(const CharacterSubrace& character_subrace) override;
    virtual void operator()(const Item& item) override;
    virtual void operator()(const Spell& spell) override;
    virtual void operator()(const Feature& feature) override;
    virtual void operator()(const Choosable& choosable) override;
};

} // namespace dnd

#endif // DISPLAY_VISITOR_HPP_
