#ifndef FILTER_SETTING_VISITOR_HPP_
#define FILTER_SETTING_VISITOR_HPP_

#include <dnd_config.hpp>

#include <core/visitors/filters/content_filter_visitor.hpp>

namespace dnd {

class FilterSettingVisitor : public ContentFilterVisitor {
public:
    void operator()(ContentPieceFilter& content_piece_filter) override;
    void operator()(CharacterFilter& character_filter) override;
    void operator()(CharacterClassFilter& character_class_filter) override;
    void operator()(CharacterSubclassFilter& character_subclass_filter) override;
    void operator()(CharacterRaceFilter& character_race_filter) override;
    void operator()(CharacterSubraceFilter& character_subrace_filter) override;
    void operator()(ItemFilter& item_filter) override;
    void operator()(SpellFilter& spell_filter) override;
    /* void operator()(FeatureFilter& feature_filter) override; */
    /* void operator()(ChoosableFilter& choosable_filter) override; */
};

} // namespace dnd

#endif // FILTER_SETTING_VISITOR_HPP_
