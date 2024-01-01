#ifndef ABILITY_SCORES_DATA_HPP_
#define ABILITY_SCORES_DATA_HPP_

#include <dnd_config.hpp>

#include <array>
#include <compare>

#include <core/errors/errors.hpp>

namespace dnd {

class Content;

class AbilityScoresData {
public:
    std::strong_ordering operator<=>(const AbilityScoresData&) const noexcept = default;

    std::array<int, 6> ability_scores;
};

Errors validate_ability_scores(const AbilityScoresData& data);

} // namespace dnd

#endif // ABILITY_SCORES_DATA_HPP_
