#include <dnd_config.hpp>

#include "ability_scores_data.hpp"

#include <array>

#include <fmt/format.h>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

AbilityScoresData::AbilityScoresData(std::shared_ptr<const ValidationData> parent) noexcept
    : ValidationSubdata(parent) {}

static Errors check_ability_score(int ability_score, std::shared_ptr<const ValidationData> parent, const char* name) {
    Errors errors;
    if (ability_score <= 0 || ability_score > 30) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, parent,
            fmt::format("Ability score {} ({}) is not between 1 and 30 (inclusive).", name, ability_score)
        );
    }

    return errors;
}

Errors validate_ability_scores(const AbilityScoresData& data) {
    Errors errors;
    errors += check_ability_score(data.ability_scores[0], data.get_parent(), "strength");
    errors += check_ability_score(data.ability_scores[1], data.get_parent(), "dexterity");
    errors += check_ability_score(data.ability_scores[2], data.get_parent(), "constitution");
    errors += check_ability_score(data.ability_scores[3], data.get_parent(), "intelligence");
    errors += check_ability_score(data.ability_scores[4], data.get_parent(), "wisdom");
    errors += check_ability_score(data.ability_scores[5], data.get_parent(), "charisma");
    return errors;
}

} // namespace dnd
