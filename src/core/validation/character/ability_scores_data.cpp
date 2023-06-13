#include <dnd_config.hpp>

#include "ability_scores_data.hpp"

#include <array>

#include <fmt/format.h>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

dnd::Errors check_ability_score(int ability_score, const dnd::ValidationData* data_ptr, const char* name) {
    dnd::Errors errors;
    if (ability_score <= 0 || ability_score > 30) {
        errors.add_validation_error(
            dnd::ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, data_ptr,
            fmt::format("Ability score {} ({}) is not between 1 and 30 (inclusive).", name, ability_score)
        );
    }

    return errors;
}

dnd::AbilityScoresData::AbilityScoresData(const ValidationData* parent) noexcept : ValidationSubdata(parent) {}

dnd::Errors dnd::AbilityScoresData::validate() const {
    Errors errors = ValidationSubdata::validate();
    errors.merge(check_ability_score(ability_scores[0], parent, "strength"));
    errors.merge(check_ability_score(ability_scores[1], parent, "dexterity"));
    errors.merge(check_ability_score(ability_scores[2], parent, "constitution"));
    errors.merge(check_ability_score(ability_scores[3], parent, "intelligence"));
    errors.merge(check_ability_score(ability_scores[4], parent, "wisdom"));
    errors.merge(check_ability_score(ability_scores[5], parent, "charisma"));
    return errors;
}
