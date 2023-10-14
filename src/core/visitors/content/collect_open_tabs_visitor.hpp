#ifndef COLLECT_OPEN_TABS_VISITOR_HPP_
#define COLLECT_OPEN_TABS_VISITOR_HPP_

#include <dnd_config.hpp>

#include <nlohmann/json.hpp>

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

/**
 * @brief A visitor that writes the names of all open tabs to a json object grouped by type.
 */
class CollectOpenTabsVisitor : public ContentVisitor {
public:
    /**
     * @brief Return a json object of open tabs
     * @return the json object
     */
    nlohmann::json get_open_tabs();
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
    nlohmann::json open_tabs_json;
};

inline nlohmann::json CollectOpenTabsVisitor::get_open_tabs() { return std::move(open_tabs_json); }

} // namespace dnd

#endif // COLLECT_OPEN_TABS_VISITOR_HPP_
