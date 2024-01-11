#ifndef ABILITY_SCORES_VALIDATION_HPP_
#define ABILITY_SCORES_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <core/errors/errors.hpp>
#include <core/models/character/ability_scores.hpp>

namespace dnd {

Errors validate_ability_scores(const AbilityScores::Data& data);

} // namespace dnd

#endif // ABILITY_SCORES_VALIDATION_HPP_
