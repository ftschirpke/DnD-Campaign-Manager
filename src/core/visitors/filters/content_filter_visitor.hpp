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
    virtual void visit(ContentPieceFilter& content_piece_filter) = 0;
    /* virtual void visit(CharacterFilter& character_filter) = 0; */
    /* virtual void visit(CharacterClassFilter& character_class_filter) = 0; */
    /* virtual void visit(CharacterSubclassFilter& character_subclass_filter) = 0; */
    /* virtual void visit(CharacterRaceFilter& character_race_filter) = 0; */
    /* virtual void visit(CharacterSubraceFilter& character_subrace_filter) = 0; */
    /* virtual void visit(ItemFilter& item_filter) = 0; */
    virtual void visit(SpellFilter& spell_filter) = 0;
    /* virtual void visit(FeatureFilter& feature_filter) = 0; */
    /* virtual void visit(ChoosableFilter& choosable_filter) = 0; */
};

} // namespace dnd

#endif // CONTENT_FILTER_VISITOR_HPP_
