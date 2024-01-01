#ifndef EFFECT_HOLDER_DATA_HPP_
#define EFFECT_HOLDER_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <string>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/validation/effects/choice/choice_data.hpp>
#include <core/validation/effects/condition/condition_data.hpp>
#include <core/validation/effects/stat_change/stat_change_data.hpp>
#include <core/validation/effects/subholders/action_holder_data.hpp>
#include <core/validation/effects/subholders/extra_spells_holder_data.hpp>
#include <core/validation/effects/subholders/proficiency_holder_data.hpp>
#include <core/validation/effects/subholders/riv_holder_data.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class EffectsData : public ValidationSubdata {
public:
    EffectsData(std::shared_ptr<const ValidationData> parent) noexcept;
    std::strong_ordering operator<=>(const EffectsData&) const noexcept = default;

    std::vector<ConditionData> activation_conditions_data;
    std::vector<ChoiceData> choices_data;
    std::vector<StatChangeData> stat_changes_data;
    ActionHolderData action_holder_data;
    ExtraSpellsHolderData extra_spells_holder_data;
    ProficiencyHolderData proficiency_holder_data;
    RIVHolderData riv_holder_data;
};

Errors validate_effects_recursively_for_content(const EffectsData& data, const Content& content);

} // namespace dnd

#endif // EFFECT_HOLDER_DATA_HPP_
