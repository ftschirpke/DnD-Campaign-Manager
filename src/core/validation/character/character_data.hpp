#ifndef CHARACTER_DATA_HPP_
#define CHARACTER_DATA_HPP_

#include <dnd_config.hpp>

#include <array>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/validation/character/ability_scores_data.hpp>
#include <core/validation/character/character_basis_data.hpp>
#include <core/validation/character/decision/decision_data.hpp>
#include <core/validation/character/progression_data.hpp>
#include <core/validation/feature/feature_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class ContentHolder;

class CharacterData : public ValidationData {
public:
    /**
     * @brief Validates the data
     * @return the errors that occured during validation
     */
    virtual Errors validate() const override;
    /**
     * @brief Validates the relations of the data to other content pieces with regard to a given content holder
     * @param content the content holder to validate the relations against
     * @return the errors that occured during validation
     */
    virtual Errors validate_relations(const ContentHolder& content) const override;

    std::vector<FeatureData> features_data;
    AbilityScoresData base_ability_scores_data;
    CharacterBasisData character_basis_data;
    ProgressionData progression_data;
    std::vector<DecisionData> decisions_data;
};

} // namespace dnd

#endif // CHARACTER_DATA_HPP_
