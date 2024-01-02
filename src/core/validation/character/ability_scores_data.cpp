#include <dnd_config.hpp>

#include "ability_scores_data.hpp"

#include <array>

#include <fmt/format.h>

#include <core/errors/errors.hpp>
#include <core/models/character/ability_scores.hpp>

namespace dnd {

static Errors check_ability_score(int ability_score, const char* name) {
    Errors errors;
    if (ability_score <= 0 || ability_score > 30) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
            fmt::format("Ability score {} ({}) is not between 1 and 30 (inclusive).", name, ability_score)
        );
    }

    return errors;
}

Errors validate_ability_scores(const AbilityScores::Data& data) {
    Errors errors;
    errors += check_ability_score(data.ability_scores[0], "strength");
    errors += check_ability_score(data.ability_scores[1], "dexterity");
    errors += check_ability_score(data.ability_scores[2], "constitution");
    errors += check_ability_score(data.ability_scores[3], "intelligence");
    errors += check_ability_score(data.ability_scores[4], "wisdom");
    errors += check_ability_score(data.ability_scores[5], "charisma");
    return errors;
}

} // namespace dnd
