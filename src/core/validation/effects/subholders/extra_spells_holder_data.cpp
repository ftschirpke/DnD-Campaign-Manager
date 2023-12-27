#include <dnd_config.hpp>

#include "extra_spells_holder_data.hpp"

#include <set>
#include <string>

#include <fmt/format.h>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/models/spell/spell.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

ExtraSpellsHolderData::ExtraSpellsHolderData(const ValidationData* parent) noexcept : ValidationSubdata(parent) {}

static Errors spells_set_validate(const std::set<std::string>& spells, const ValidationData* parent) {
    Errors errors;

    for (const std::string& spell_name : spells) {
        if (spell_name.empty()) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE, parent, fmt::format("Name of extra spell is empty.")
            );
        }
    }
    return errors;
}

Errors ExtraSpellsHolderData::validate() const {
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

static Errors spells_set_validate_relations(
    const std::set<std::string>& spells, const ValidationData* parent, const Content& content
) {
    Errors errors;

    for (const std::string& spell_name : spells) {
        OptCRef<Spell> spell_optional = content.get_spells().get(spell_name);
        if (!spell_optional.has_value()) {
            errors.add_validation_error(
                ValidationError::Code::RELATION_NOT_FOUND, parent, fmt::format("Spell '{}' does not exist.", spell_name)
            );
        } else if (spell_optional.value().get().get_type().get_spell_level() == SpellLevel::CANTRIP) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_RELATION, parent, fmt::format("Spell '{}' is a cantrip.", spell_name)
            );
        }
    }
    return errors;
}

Errors ExtraSpellsHolderData::validate_relations(const Content& content) const {
    Errors errors;
    for (const std::string& cantrip_name : free_cantrips) {
        OptCRef<Spell> cantrip_optional = content.get_spells().get(cantrip_name);
        if (!cantrip_optional.has_value()) {
            errors.add_validation_error(
                ValidationError::Code::RELATION_NOT_FOUND, parent,
                fmt::format("Cantrip '{}' does not exist.", cantrip_name)
            );
        } else if (cantrip_optional.value().get().get_type().get_spell_level() != SpellLevel::CANTRIP) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_RELATION, parent,
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

bool ExtraSpellsHolderData::empty() const noexcept {
    return free_cantrips.empty() && at_will.empty() && innate.empty() && free_once_a_day.empty() && spells_known.empty()
           && spells_known_included.empty() && added_to_spell_list.empty();
}

} // namespace dnd
