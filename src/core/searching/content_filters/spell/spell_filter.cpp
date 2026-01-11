#include <dnd_config.hpp>

#include "spell_filter.hpp"

#include <algorithm>
#include <vector>

#include <core/content.hpp>
#include <core/models/spell/spell.hpp>
#include <core/models/spell/spell_components.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

namespace dnd {

bool SpellFilter::has_all_filters() const {
    return ContentPieceFilter::has_all_filters() && verbal_component_filter.is_set()
           && somatic_component_filter.is_set() && material_component_filter.is_set() && level_filter.is_set()
           && magic_school_filter.is_set() && ritual_filter.is_set() && casting_time_filter.is_set()
           && range_filter.is_set() && duration_filter.is_set() && classes_filter.is_set();
}


bool SpellFilter::matches(const Spell& spell) const {
    const SpellComponents& components = spell.get_components();
    const MagicSchool& magic_school = spell.get_type().get_magic_school();
    return ContentPieceFilter::matches(spell) && verbal_component_filter.matches(components.has_verbal())
           && somatic_component_filter.matches(components.has_somatic())
           && material_component_filter.matches(components.has_material())
           && level_filter.matches(spell.get_type().get_spell_level_as_int())
           && magic_school_filter.matches(magic_school) && ritual_filter.matches(spell.get_type().is_ritual())
           && casting_time_filter.matches(spell.get_casting_time()) && range_filter.matches(spell.get_range())
           && duration_filter.matches(spell.get_duration())
           && std::any_of(
               spell.get_classes().begin(), spell.get_classes().end(),
               [this](const std::string& class_name) { return classes_filter.matches(class_name); }
           );
}

std::vector<const ContentPiece*> SpellFilter::all_matches(const Content& content) const {
    std::vector<const ContentPiece*> matching_content_pieces;
    for (const auto& spell : content.get_spell_library().get_all()) {
        if (matches(spell)) {
            matching_content_pieces.push_back(&spell);
        }
    }
    return matching_content_pieces;
}

void SpellFilter::clear() {
    ContentPieceFilter::clear();
    verbal_component_filter.clear();
    somatic_component_filter.clear();
    material_component_filter.clear();
    level_filter.clear();
    magic_school_filter.clear();
    ritual_filter.clear();
    casting_time_filter.clear();
    range_filter.clear();
    duration_filter.clear();
    classes_filter.clear();
}

} // namespace dnd
