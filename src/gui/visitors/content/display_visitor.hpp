#ifndef DISPLAY_VISITOR_HPP_
#define DISPLAY_VISITOR_HPP_

#include <dnd_config.hpp>

#include <core/content.hpp>
#include <core/models/character/character.hpp>
#include <core/models/class/class.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/models/effects_provider/class_feature.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/effects_provider/subclass_feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/species/species.hpp>
#include <core/models/spell/spell.hpp>
#include <core/models/subclass/subclass.hpp>
#include <core/models/subspecies/subspecies.hpp>
#include <core/visitors/content/content_visitor.hpp>
#include <gui/gui_fonts.hpp>

namespace dnd {

class DisplayVisitor : public ContentVisitor {
public:
    DisplayVisitor(const Content& content, const GuiFonts& fonts);
    virtual void operator()(const Character& character) override;
    virtual void operator()(const Class& cls) override;
    virtual void operator()(const Subclass& subclass) override;
    virtual void operator()(const Species& species) override;
    virtual void operator()(const Subspecies& subspecies) override;
    virtual void operator()(const Item& item) override;
    virtual void operator()(const Spell& spell) override;
    virtual void operator()(const Feature& feature) override;
    virtual void operator()(const ClassFeature& class_feature) override;
    virtual void operator()(const SubclassFeature& subclass_feature) override;
    virtual void operator()(const Choosable& choosable) override;
private:
    const Content& content;
    const GuiFonts& fonts;
};

void display_formatted_text(const Text& formatted_text, const GuiFonts& fonts);

} // namespace dnd

#endif // DISPLAY_VISITOR_HPP_
