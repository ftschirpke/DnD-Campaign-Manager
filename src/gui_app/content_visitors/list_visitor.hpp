#ifndef LIST_VISITOR_HPP_
#define LIST_VISITOR_HPP_

#include <dnd_config.hpp>

#include <string>
#include <utility>
#include <vector>

#include <core/content_visitors/content_visitor.hpp>
#include <core/models/character.hpp>
#include <core/models/character_class.hpp>
#include <core/models/character_race.hpp>
#include <core/models/character_subclass.hpp>
#include <core/models/character_subrace.hpp>
#include <core/models/effect_holder/choosable.hpp>
#include <core/models/effect_holder/feature.hpp>
#include <core/models/item.hpp>
#include <core/models/spell.hpp>

namespace dnd {

/**
 * @brief A visitor for creating a list of content pieces (name and type for each)
 */
class ListVisitor : public ContentVisitor {
public:
    /**
     * @brief Return a vector of list-suitable strings
     * @return the vector of strings
     */
    std::vector<std::string> get_list();
    virtual void visit(const Character* character_ptr) override;
    virtual void visit(const CharacterClass* character_class_ptr) override;
    virtual void visit(const CharacterSubclass* character_subclass_ptr) override;
    virtual void visit(const CharacterRace* character_race_ptr) override;
    virtual void visit(const CharacterSubrace* character_subrace_ptr) override;
    virtual void visit(const Item* item_ptr) override;
    virtual void visit(const Spell* spell_ptr) override;
    virtual void visit(const Feature* feature_ptr) override;
    virtual void visit(const Choosable* choosable_ptr) override;
private:
    std::vector<std::string> string_list;
};

inline std::vector<std::string> ListVisitor::get_list() { return std::move(string_list); }

} // namespace dnd

#endif // LIST_VISITOR_HPP_
