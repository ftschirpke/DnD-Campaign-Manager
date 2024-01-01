#ifndef DATA_HPP_
#define DATA_HPP_

#include <dnd_config.hpp>

#include <array>
#include <compare>
#include <memory>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/validation/character/ability_scores_data.hpp>
#include <core/validation/character/decision/decision_data.hpp>
#include <core/validation/character/feature_providers_data.hpp>
#include <core/validation/character/progression_data.hpp>
#include <core/validation/effects_provider/feature_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class Content;

class CharacterData : public ValidationData {
public:
    std::strong_ordering operator<=>(const CharacterData&) const noexcept = default;

    std::vector<FeatureData> features_data;
    AbilityScoresData base_ability_scores_data;
    FeatureProvidersData feature_providers_data;
    ProgressionData progression_data;
    std::vector<DecisionData> decisions_data;
};

Errors validate_character_nonrecursively_for_content(const CharacterData& data, const Content& content);
Errors validate_character_recursively_for_content(const CharacterData& data, const Content& content);

} // namespace dnd

#endif // DATA_HPP_
