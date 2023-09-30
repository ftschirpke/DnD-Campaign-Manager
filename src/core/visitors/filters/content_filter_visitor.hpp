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

class ContentFilterVisitor {
public:
    virtual ~ContentFilterVisitor() = default;
    virtual void visit(const ContentPieceFilter& content_piece_filter) = 0;
    virtual void visit(const CharacterFilter& character_filter) = 0;
    virtual void visit(const CharacterClassFilter& character_class_filter) = 0;
    virtual void visit(const CharacterSubclassFilter& character_subclass_filter) = 0;
    virtual void visit(const CharacterRaceFilter& character_race_filter) = 0;
    virtual void visit(const CharacterSubraceFilter& character_subrace_filter) = 0;
    virtual void visit(const ItemFilter& item_filter) = 0;
    virtual void visit(const SpellFilter& spell_filter) = 0;
    virtual void visit(const FeatureFilter& feature_filter) = 0;
    virtual void visit(const ChoosableFilter& choosable_filter) = 0;
};

} // namespace dnd

#endif // CONTENT_FILTER_VISITOR_HPP_
