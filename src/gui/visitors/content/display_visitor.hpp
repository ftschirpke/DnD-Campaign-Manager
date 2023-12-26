#ifndef DISPLAY_VISITOR_HPP_
#define DISPLAY_VISITOR_HPP_

#include <dnd_config.hpp>

#include <core/models/character/character.hpp>
#include <core/models/class/class.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/species/species.hpp>
#include <core/models/spell/spell.hpp>
#include <core/models/subclass/subclass.hpp>
#include <core/models/subspecies/subspecies.hpp>
#include <core/output/string_formatting/string_formatter.hpp>
#include <core/visitors/content/content_visitor.hpp>
#include <gui/string_formatting/display_format_visitor.hpp>

namespace dnd {

class DisplayVisitor : public ContentVisitor {
public:
    virtual void operator()(const Character& character) override;
    virtual void operator()(const Class& classv) override;
    virtual void operator()(const Subclass& subclass) override;
    virtual void operator()(const Species& species) override;
    virtual void operator()(const Subspecies& subspecies) override;
    virtual void operator()(const Item& item) override;
    virtual void operator()(const Spell& spell) override;
    virtual void operator()(const Feature& feature) override;
    virtual void operator()(const Choosable& choosable) override;
};

} // namespace dnd

#endif // DISPLAY_VISITOR_HPP_
