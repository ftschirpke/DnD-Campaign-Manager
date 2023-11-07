#ifndef CONTENT_FILTER_VISITOR_HPP_
#define CONTENT_FILTER_VISITOR_HPP_

#include <dnd_config.hpp>

namespace dnd {

class ContentPieceFilter;
class CharacterFilter;
class CharacterClassFilter;
class CharacterSubclassFilter;
class CharacterClassFilter;
class CharacterRaceFilter;
class CharacterSubraceFilter;
class ItemFilter;
class SpellFilter;
class FeatureFilter;
class ChoosableFilter;

// TODO: deprecated?

class ContentFilterVisitor {
public:
    virtual ~ContentFilterVisitor() = default;
    virtual void operator()(ContentPieceFilter& content_piece_filter) = 0;
    virtual void operator()(CharacterFilter& character_filter) = 0;
    /* virtual void operator()(CharacterClassFilter& character_class_filter) = 0; */
    /* virtual void operator()(CharacterSubclassFilter& character_subclass_filter) = 0; */
    /* virtual void operator()(CharacterRaceFilter& character_race_filter) = 0; */
    /* virtual void operator()(CharacterSubraceFilter& character_subrace_filter) = 0; */
    /* virtual void operator()(ItemFilter& item_filter) = 0; */
    virtual void operator()(SpellFilter& spell_filter) = 0;
    /* virtual void operator()(FeatureFilter& feature_filter) = 0; */
    /* virtual void operator()(ChoosableFilter& choosable_filter) = 0; */
};

} // namespace dnd

#endif // CONTENT_FILTER_VISITOR_HPP_
