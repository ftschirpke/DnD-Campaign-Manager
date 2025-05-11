#ifndef CONTENT_VISITOR_HPP_
#define CONTENT_VISITOR_HPP_

#include <dnd_config.hpp>

#include <core/models/character/character.hpp>
#include <core/models/class/class.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/models/effects_provider/class_feature.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/species/species.hpp>
#include <core/models/spell/spell.hpp>
#include <core/models/subclass/subclass.hpp>
#include <core/models/subspecies/subspecies.hpp>

namespace dnd {

class ContentVisitor {
public:
    virtual ~ContentVisitor() = default;
    virtual void operator()(const Character& character) = 0;
    virtual void operator()(const Class& cls) = 0;
    virtual void operator()(const Subclass& subclass) = 0;
    virtual void operator()(const Species& species) = 0;
    virtual void operator()(const Subspecies& subspecies) = 0;
    virtual void operator()(const Item& item) = 0;
    virtual void operator()(const Spell& spell) = 0;
    virtual void operator()(const Feature& feature) = 0;
    virtual void operator()(const ClassFeature& class_feature) = 0;
    virtual void operator()(const SubclassFeature& class_feature) = 0;
    virtual void operator()(const Choosable& choosable) = 0;
};

} // namespace dnd

#endif // CONTENT_VISITOR_HPP_
