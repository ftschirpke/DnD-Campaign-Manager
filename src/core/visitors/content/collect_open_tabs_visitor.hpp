#ifndef COLLECT_OPEN_TABS_VISITOR_HPP_
#define COLLECT_OPEN_TABS_VISITOR_HPP_

#include <dnd_config.hpp>

#include <nlohmann/json.hpp>

#include <core/models/character/character.hpp>
#include <core/models/class/class.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/species/species.hpp>
#include <core/models/spell/spell.hpp>
#include <core/models/subclass/subclass.hpp>
#include <core/models/subspecies/subspecies.hpp>
#include <core/visitors/content/content_visitor.hpp>

namespace dnd {

class CollectOpenTabsVisitor : public ContentVisitor {
public:
    nlohmann::json get_open_tabs();
    virtual void operator()(const Character& character) override;
    virtual void operator()(const Class& classv) override;
    virtual void operator()(const Subclass& subclass) override;
    virtual void operator()(const Species& species) override;
    virtual void operator()(const Subspecies& subspecies) override;
    virtual void operator()(const Item& item) override;
    virtual void operator()(const Spell& spell) override;
    virtual void operator()(const Feature& feature) override;
    virtual void operator()(const Choosable& choosable) override;
private:
    nlohmann::json open_tabs_json;
};

} // namespace dnd

#endif // COLLECT_OPEN_TABS_VISITOR_HPP_
