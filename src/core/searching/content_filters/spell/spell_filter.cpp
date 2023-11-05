#include <dnd_config.hpp>

#include "spell_filter.hpp"

#include <algorithm>
#include <vector>

#include <core/models/spell/spell.hpp>
#include <core/models/spell/spell_components.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

bool dnd::SpellFilter::has_verbal_component_filter() const noexcept { return verbal_component_filter.is_set(); }

bool dnd::SpellFilter::has_somatic_component_filter() const noexcept { return somatic_component_filter.is_set(); }

bool dnd::SpellFilter::has_material_component_filter() const noexcept { return material_component_filter.is_set(); }

bool dnd::SpellFilter::has_level_filter() const noexcept { return level_filter.is_set(); }

bool dnd::SpellFilter::has_magic_school_filter() const noexcept { return magic_school_filter.is_set(); }

bool dnd::SpellFilter::has_ritual_filter() const noexcept { return ritual_filter.is_set(); }

bool dnd::SpellFilter::has_casting_time_filter() const noexcept { return casting_time_filter.is_set(); }

bool dnd::SpellFilter::has_range_filter() const noexcept { return range_filter.is_set(); }

bool dnd::SpellFilter::has_duration_filter() const noexcept { return duration_filter.is_set(); }

bool dnd::SpellFilter::has_classes_filter() const noexcept { return classes_filter.is_set(); }

dnd::BoolFilter& dnd::SpellFilter::get_verbal_component_filter() noexcept { return verbal_component_filter; }

dnd::BoolFilter& dnd::SpellFilter::get_somatic_component_filter() noexcept { return somatic_component_filter; }

dnd::BoolFilter& dnd::SpellFilter::get_material_component_filter() noexcept { return material_component_filter; }

dnd::NumberFilter<int>& dnd::SpellFilter::get_level_filter() noexcept { return level_filter; }

dnd::SelectionFilter<dnd::MagicSchool>& dnd::SpellFilter::get_magic_school_filter() noexcept {
    return magic_school_filter;
}

dnd::BoolFilter& dnd::SpellFilter::get_ritual_filter() noexcept { return ritual_filter; }

dnd::StringFilter& dnd::SpellFilter::get_casting_time_filter() noexcept { return casting_time_filter; }

dnd::StringFilter& dnd::SpellFilter::get_range_filter() noexcept { return range_filter; }

dnd::StringFilter& dnd::SpellFilter::get_duration_filter() noexcept { return duration_filter; }

dnd::SelectionFilter<std::string>& dnd::SpellFilter::get_classes_filter() noexcept { return classes_filter; }

bool dnd::SpellFilter::matches(const Spell& spell) const noexcept {
    const SpellComponents& components = spell.get_components();
    const MagicSchool& magic_school = spell.get_type().get_magic_school();
    return ContentPieceFilter::matches(spell) && verbal_component_filter.matches(components.has_verbal())
           && somatic_component_filter.matches(components.has_somatic())
           && material_component_filter.matches(components.has_material())
           && level_filter.matches(spell.get_type().get_spell_level_int()) && magic_school_filter.matches(magic_school)
           && ritual_filter.matches(spell.get_type().is_ritual())
           && casting_time_filter.matches(spell.get_casting_time()) && range_filter.matches(spell.get_range())
           && duration_filter.matches(spell.get_duration())
           && std::any_of(
               spell.get_classes().begin(), spell.get_classes().end(),
               [this](const std::string& class_name) { return classes_filter.matches(class_name); }
           );
}

void dnd::SpellFilter::clear() {
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
