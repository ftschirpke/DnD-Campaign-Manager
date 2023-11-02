#ifndef FILTER_SETTING_VISITOR_HPP_
#define FILTER_SETTING_VISITOR_HPP_

#include <dnd_config.hpp>

#include <core/visitors/filters/content_filter_visitor.hpp>

namespace dnd {

class FilterSettingVisitor : public ContentFilterVisitor {
public:
    void visit(ContentPieceFilter& content_piece_filter) override;
    /* void visit(CharacterFilter& character_filter) override; */
    /* void visit(CharacterClassFilter& character_class_filter) override; */
    /* void visit(CharacterSubclassFilter& character_subclass_filter) override; */
    /* void visit(CharacterRaceFilter& character_race_filter) override; */
    /* void visit(CharacterSubraceFilter& character_subrace_filter) override; */
    /* void visit(ItemFilter& item_filter) override; */
    void visit(SpellFilter& spell_filter) override;
    /* void visit(FeatureFilter& feature_filter) override; */
    /* void visit(ChoosableFilter& choosable_filter) override; */
};

} // namespace dnd

#endif // FILTER_SETTING_VISITOR_HPP_
