#ifndef CHARACTER_DATA_HPP_
#define CHARACTER_DATA_HPP_

#include <compare>
#include <vector>

#include <core/models/character/ability_scores.hpp>
#include <core/models/character/character_without_data.hpp>
#include <core/models/character/decision/decision.hpp>
#include <core/models/character/feature_providers.hpp>
#include <core/models/character/progression.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class Character::Data : public ValidationData {
public:
    std::strong_ordering operator<=>(const Character::Data&) const noexcept = default;

    std::vector<Feature::Data> features_data;
    AbilityScores::Data base_ability_scores_data;
    FeatureProviders::Data feature_providers_data;
    Progression::Data progression_data;
    std::vector<Decision::Data> decisions_data;
};

} // namespace dnd

#endif // CHARACTER_DATA_HPP_
