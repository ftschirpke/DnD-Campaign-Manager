#ifndef FILTER_SETTING_VISITOR_HPP_
#define FILTER_SETTING_VISITOR_HPP_

#include <dnd_config.hpp>

#include <core/searching/content_filters/character/character_filter.hpp>
#include <core/searching/content_filters/character_class/character_class_filter.hpp>
#include <core/searching/content_filters/character_species/character_species_filter.hpp>
#include <core/searching/content_filters/character_subclass/character_subclass_filter.hpp>
#include <core/searching/content_filters/character_subspecies/character_subspecies_filter.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>
#include <core/searching/content_filters/effects_provider/choosable_filter.hpp>
#include <core/searching/content_filters/effects_provider/feature_filter.hpp>
#include <core/searching/content_filters/item/item_filter.hpp>
#include <core/searching/content_filters/spell/spell_filter.hpp>


namespace dnd {

class FilterSettingVisitor {
public:
    void operator()(ContentPieceFilter& content_piece_filter);
    void operator()(CharacterFilter& character_filter);
    void operator()(CharacterClassFilter& character_class_filter);
    void operator()(CharacterSubclassFilter& character_subclass_filter);
    void operator()(CharacterSpeciesFilter& character_species_filter);
    void operator()(CharacterSubspeciesFilter& character_subspecies_filter);
    void operator()(ItemFilter& item_filter);
    void operator()(SpellFilter& spell_filter);
    void operator()(FeatureFilter& feature_filter);
    void operator()(ChoosableFilter& choosable_filter);
};

} // namespace dnd

#endif // FILTER_SETTING_VISITOR_HPP_
