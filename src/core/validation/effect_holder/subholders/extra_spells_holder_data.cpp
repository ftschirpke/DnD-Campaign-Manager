#include <dnd_config.hpp>

#include "extra_spells_holder_data.hpp"

#include <set>
#include <string>

#include <fmt/format.h>

#include <core/controllers/content_holder.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/models/spell/spell.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

dnd::ExtraSpellsHolderData::ExtraSpellsHolderData(const ValidationData* parent) noexcept : ValidationSubdata(parent) {}

static dnd::Errors spells_set_validate(const std::set<std::string>& spells, const dnd::ValidationData* parent) {
    dnd::Errors errors;

    for (const auto& spell_name : spells) {
        if (spell_name.empty()) {
            errors.add_validation_error(
                dnd::ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent, fmt::format("Name of extra spell is empty.")
            );
        }
    }
    return errors;
}

dnd::Errors dnd::ExtraSpellsHolderData::validate() const {
    Errors errors;
    errors += spells_set_validate(free_cantrips, parent);
    errors += spells_set_validate(at_will, parent);
    errors += spells_set_validate(innate, parent);
    errors += spells_set_validate(free_once_a_day, parent);
    errors += spells_set_validate(spells_known, parent);
    errors += spells_set_validate(spells_known_included, parent);
    errors += spells_set_validate(added_to_spell_list, parent);
    return errors;
}

static dnd::Errors spells_set_validate_relations(
    const std::set<std::string>& spells, const dnd::ValidationData* parent, const dnd::ContentHolder& content
) {
    dnd::Errors errors;

    for (const auto& spell_name : spells) {
        if (!content.spells.contains(spell_name)) {
            errors.add_validation_error(
                dnd::ValidationErrorCode::RELATION_NOT_FOUND, parent,
                fmt::format("Spell '{}' does not exist.", spell_name)
            );
        }
    }
    return errors;
}

dnd::Errors dnd::ExtraSpellsHolderData::validate_relations(const ContentHolder& content) const {
    Errors errors;
    for (const auto& cantrip_name : free_cantrips) {
        if (!content.spells.contains(cantrip_name)) {
            errors.add_validation_error(
                dnd::ValidationErrorCode::RELATION_NOT_FOUND, parent,
                fmt::format("Cantrip '{}' does not exist.", cantrip_name)
            );
        } else if (content.spells.get(cantrip_name).get_type().get_spell_level() != dnd::SpellLevel::CANTRIP) {
            errors.add_validation_error(
                dnd::ValidationErrorCode::INVALID_RELATION, parent,
                fmt::format("Spell '{}' is not a cantrip.", cantrip_name)
            );
        }
    }
    errors += spells_set_validate_relations(at_will, parent, content);
    errors += spells_set_validate_relations(innate, parent, content);
    errors += spells_set_validate_relations(free_once_a_day, parent, content);
    errors += spells_set_validate_relations(spells_known, parent, content);
    errors += spells_set_validate_relations(spells_known_included, parent, content);
    errors += spells_set_validate_relations(added_to_spell_list, parent, content);
    return errors;
}

bool dnd::ExtraSpellsHolderData::empty() const noexcept {
    return free_cantrips.empty() && at_will.empty() && innate.empty() && free_once_a_day.empty() && spells_known.empty()
           && spells_known_included.empty() && added_to_spell_list.empty();
}
