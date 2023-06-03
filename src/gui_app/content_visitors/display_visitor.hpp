#ifndef DISPLAY_VISITOR_HPP_
#define DISPLAY_VISITOR_HPP_

#include "dnd_config.hpp"

#include "core/content_visitors/content_visitor.hpp"
#include "core/models/character.hpp"
#include "core/models/character_class.hpp"
#include "core/models/character_race.hpp"
#include "core/models/character_subclass.hpp"
#include "core/models/character_subrace.hpp"
#include "core/models/effect_holder/choosable.hpp"
#include "core/models/effect_holder/feature.hpp"
#include "core/models/item.hpp"
#include "core/models/spell.hpp"
#include "core/output/string_formatting/string_formatter.hpp"
#include "gui_app/string_formatting/display_format_visitor.hpp"

namespace dnd {

/**
 * @brief A visitor that displays the content using ImGui widgets
 */
class DisplayVisitor : public ContentVisitor {
public:
    DisplayVisitor() noexcept;
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
    void display_formatted_text(const std::string& formatted_text);
    void list_features(const FeatureHolder* feature_holder_ptr);

    DisplayFormatVisitor display_format_visitor;
    StringFormatter string_formatter;

    static constexpr ImGuiTableFlags table_flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg
                                                   | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX;
};

inline DisplayVisitor::DisplayVisitor() noexcept : display_format_visitor(table_flags), string_formatter(false) {}

} // namespace dnd

#endif // DISPLAY_VISITOR_HPP_