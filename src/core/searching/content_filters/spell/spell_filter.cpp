#include <dnd_config.hpp>

#include "spell_filter.hpp"

#include <set>

#include <core/models/spell/spell.hpp>
#include <core/models/spell/spell_components.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

dnd::SpellFilter::SpellFilter() noexcept
    : ContentPieceFilter(), verbal_component_filter_type(BoolFilterType::NONE),
      somatic_component_filter_type(BoolFilterType::NONE), material_component_filter_type(BoolFilterType::NONE),
      level_filter_type(NumberFilterType::NONE), level_filter_level(0),
      magic_school_filter_type(SelectionFilterType::NONE), ritual_filter_type(BoolFilterType::NONE),
      classes_filter_type(SelectionFilterType::NONE) {}

void dnd::SpellFilter::accept(ContentFilterVisitor& visitor) { visitor.visit(*this); }

void dnd::SpellFilter::set_verbal_component_filter(BoolFilterType type) noexcept {
    verbal_component_filter_type = type;
}

void dnd::SpellFilter::remove_verbal_component_filter() noexcept {
    verbal_component_filter_type = BoolFilterType::NONE;
}

void dnd::SpellFilter::set_somatic_component_filter(BoolFilterType type) noexcept {
    somatic_component_filter_type = type;
}

void dnd::SpellFilter::remove_somatic_component_filter() noexcept {
    somatic_component_filter_type = BoolFilterType::NONE;
}

void dnd::SpellFilter::set_material_component_filter(BoolFilterType type) noexcept {
    material_component_filter_type = type;
}

void dnd::SpellFilter::remove_material_component_filter() noexcept {
    material_component_filter_type = BoolFilterType::NONE;
}

void dnd::SpellFilter::set_level_filter(NumberFilterType type, int level) noexcept {
    level_filter_type = type;
    level_filter_level = level;
}

void dnd::SpellFilter::remove_level_filter() noexcept {
    level_filter_type = NumberFilterType::NONE;
    level_filter_level = 0;
}

void dnd::SpellFilter::set_magic_school_filter(SelectionFilterType type, std::set<MagicSchool> magic_schools) noexcept {
    magic_school_filter_type = type;
    magic_school_filter_magic_schools = magic_schools;
}

void dnd::SpellFilter::remove_magic_school_filter() noexcept { magic_school_filter_type = SelectionFilterType::NONE; }

void dnd::SpellFilter::set_ritual_filter(BoolFilterType type) noexcept { ritual_filter_type = type; }

void dnd::SpellFilter::remove_ritual_filter() noexcept { ritual_filter_type = BoolFilterType::NONE; }

void dnd::SpellFilter::set_classes_filter(SelectionFilterType type, std::set<std::string> classes) noexcept {
    classes_filter_type = type;
    classes_filter_classes = classes;
}

void dnd::SpellFilter::remove_classes_filter() noexcept { classes_filter_type = SelectionFilterType::NONE; }

bool dnd::SpellFilter::matches(const Spell& spell) const noexcept {
    const SpellComponents& components = spell.get_components();
    if (!matches_bool(components.has_verbal(), verbal_component_filter_type)) {
        return false;
    }
    if (!matches_bool(components.has_somatic(), somatic_component_filter_type)) {
        return false;
    }
    if (!matches_bool(components.has_material(), material_component_filter_type)) {
        return false;
    }
    if (!matches_int(spell.get_type().get_spell_level_int(), level_filter_type, level_filter_level)) {
        return false;
    }
    if (!matches_bool(spell.get_type().is_ritual(), ritual_filter_type)) {
        return false;
    }
    return matches_selection(
        spell.get_type().get_magic_school(), magic_school_filter_type, magic_school_filter_magic_schools
    );
}
