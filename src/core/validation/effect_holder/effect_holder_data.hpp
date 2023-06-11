#ifndef EFFECT_HOLDER_DATA_HPP_
#define EFFECT_HOLDER_DATA_HPP_

#include <dnd_config.hpp>

#include <string>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/validation/effect_holder/condition/condition_data.hpp>
#include <core/validation/effect_holder/effect/effect_data.hpp>
#include <core/validation/effect_holder/subholders/action_holder_data.hpp>
#include <core/validation/effect_holder/subholders/extra_spells_holder_data.hpp>
#include <core/validation/effect_holder/subholders/proficiency_holder_data.hpp>
#include <core/validation/effect_holder/subholders/riv_holder_data.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class EffectHolderData : public ValidationSubdata {
public:
    EffectHolderData(const ValidationData* parent) noexcept;
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
    virtual Errors validate_relations(const ContentHolder* content) const override;

    std::vector<ConditionData> activation_conditions;
    std::vector<EffectData> effects;
    ActionHolderData action_holder_data;
    ExtraSpellsHolderData extra_spells_holder_data;
    ProficiencyHolderData proficiency_holder_data;
    RIVHolderData riv_holder_data;
};

} // namespace dnd

#endif // EFFECT_HOLDER_DATA_HPP_
