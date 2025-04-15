#include <dnd_config.hpp>

#include "extra_spells_holder_validation.hpp"

#include <fmt/format.h>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/models/effects/subholders/extra_spells_holder.hpp>
#include <core/models/spell/spell.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

static Errors spells_set_validate(const std::set<std::string>& spells) {
    Errors errors;

    for (const std::string& spell_name : spells) {
        if (spell_name.empty()) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE, fmt::format("Name of extra spell is empty.")
            );
        }
    }
    return errors;
}

static Errors validate_extra_spells_holder_raw(const ExtraSpellsHolder::Data& data) {
    Errors errors;
    errors += spells_set_validate(data.free_cantrips);
    errors += spells_set_validate(data.at_will);
    errors += spells_set_validate(data.innate);
    errors += spells_set_validate(data.free_once_a_day);
    errors += spells_set_validate(data.spells_known);
    errors += spells_set_validate(data.spells_known_included);
    errors += spells_set_validate(data.added_to_spell_list);
    return errors;
}

static Errors spells_set_validate_relations(const std::set<std::string>& spells, const Content& content) {
    Errors errors;

    for (const std::string& spell_key : spells) {
        OptCRef<Spell> spell_optional = content.get_spells().get(spell_key);
        if (!spell_optional.has_value()) {
            errors.add_validation_error(
                ValidationError::Code::RELATION_NOT_FOUND, fmt::format("Spell '{}' does not exist.", spell_key)
            );
        } else if (spell_optional.value().get().get_type().get_spell_level() == SpellLevel::CANTRIP) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_RELATION, fmt::format("Spell '{}' is a cantrip.", spell_key)
            );
        }
    }
    return errors;
}

static Errors validate_extra_spells_holder_relations(const ExtraSpellsHolder::Data& data, const Content& content) {
    Errors errors;
    for (const std::string& cantrip_name : data.free_cantrips) {
        OptCRef<Spell> cantrip_optional = content.get_spells().get(cantrip_name);
        if (!cantrip_optional.has_value()) {
            errors.add_validation_error(
                ValidationError::Code::RELATION_NOT_FOUND, fmt::format("Cantrip '{}' does not exist.", cantrip_name)
            );
        } else if (cantrip_optional.value().get().get_type().get_spell_level() != SpellLevel::CANTRIP) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_RELATION, fmt::format("Spell '{}' is not a cantrip.", cantrip_name)
            );
        }
    }
    errors += spells_set_validate_relations(data.at_will, content);
    errors += spells_set_validate_relations(data.innate, content);
    errors += spells_set_validate_relations(data.free_once_a_day, content);
    errors += spells_set_validate_relations(data.spells_known, content);
    errors += spells_set_validate_relations(data.spells_known_included, content);
    errors += spells_set_validate_relations(data.added_to_spell_list, content);
    return errors;
}

Errors validate_extra_spells_holder_for_content(const ExtraSpellsHolder::Data& data, const Content& content) {
    Errors errors = validate_extra_spells_holder_raw(data);
    errors += validate_extra_spells_holder_relations(data, content);
    return errors;
}

} // namespace dnd
