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

ExtraSpellsHolderData::ExtraSpellsHolderData(std::shared_ptr<const ValidationData> parent) noexcept
    : ValidationSubdata(parent) {}

bool ExtraSpellsHolderData::empty() const noexcept {
    return free_cantrips.empty() && at_will.empty() && innate.empty() && free_once_a_day.empty() && spells_known.empty()
           && spells_known_included.empty() && added_to_spell_list.empty();
}

static Errors spells_set_validate(const std::set<std::string>& spells, std::shared_ptr<const ValidationData> parent) {
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

static Errors validate_extra_spells_holder_raw(const ExtraSpellsHolderData& data) {
    Errors errors;
    errors += spells_set_validate(data.free_cantrips, data.get_parent());
    errors += spells_set_validate(data.at_will, data.get_parent());
    errors += spells_set_validate(data.innate, data.get_parent());
    errors += spells_set_validate(data.free_once_a_day, data.get_parent());
    errors += spells_set_validate(data.spells_known, data.get_parent());
    errors += spells_set_validate(data.spells_known_included, data.get_parent());
    errors += spells_set_validate(data.added_to_spell_list, data.get_parent());
    return errors;
}

static Errors spells_set_validate_relations(
    const std::set<std::string>& spells, std::shared_ptr<const ValidationData> parent, const Content& content
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

static Errors validate_extra_spells_holder_relations(const ExtraSpellsHolderData& data, const Content& content) {
    Errors errors;
    for (const std::string& cantrip_name : data.free_cantrips) {
        OptCRef<Spell> cantrip_optional = content.get_spells().get(cantrip_name);
        if (!cantrip_optional.has_value()) {
            errors.add_validation_error(
                ValidationError::Code::RELATION_NOT_FOUND, data.get_parent(),
                fmt::format("Cantrip '{}' does not exist.", cantrip_name)
            );
        } else if (cantrip_optional.value().get().get_type().get_spell_level() != SpellLevel::CANTRIP) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_RELATION, data.get_parent(),
                fmt::format("Spell '{}' is not a cantrip.", cantrip_name)
            );
        }
    }
    errors += spells_set_validate_relations(data.at_will, data.get_parent(), content);
    errors += spells_set_validate_relations(data.innate, data.get_parent(), content);
    errors += spells_set_validate_relations(data.free_once_a_day, data.get_parent(), content);
    errors += spells_set_validate_relations(data.spells_known, data.get_parent(), content);
    errors += spells_set_validate_relations(data.spells_known_included, data.get_parent(), content);
    errors += spells_set_validate_relations(data.added_to_spell_list, data.get_parent(), content);
    return errors;
}

Errors validate_extra_spells_holder_for_content(const ExtraSpellsHolderData& data, const Content& content) {
    Errors errors = validate_extra_spells_holder_raw(data);
    errors += validate_extra_spells_holder_relations(data, content);
    return errors;
}

} // namespace dnd
