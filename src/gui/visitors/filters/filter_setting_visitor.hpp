#ifndef FILTER_SETTING_VISITOR_HPP_
#define FILTER_SETTING_VISITOR_HPP_

#include <dnd_config.hpp>

#include <core/visitors/filters/content_filter_visitor.hpp>

namespace dnd {

class FilterSettingVisitor {
public:
    void operator()(ContentPieceFilter& content_piece_filter);
    /* void operator()(CharacterFilter& character_filter) ; */
    /* void operator()(CharacterClassFilter& character_class_filter) ; */
    /* void operator()(CharacterSubclassFilter& character_subclass_filter) ; */
    /* void operator()(CharacterRaceFilter& character_race_filter) ; */
    /* void operator()(CharacterSubraceFilter& character_subrace_filter) ; */
    /* void operator()(ItemFilter& item_filter) ; */
    void operator()(SpellFilter& spell_filter);
    /* void operator()(FeatureFilter& feature_filter) ; */
    /* void operator()(ChoosableFilter& choosable_filter) ; */
};

} // namespace dnd

#endif // FILTER_SETTING_VISITOR_HPP_
