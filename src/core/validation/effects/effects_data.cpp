#include <dnd_config.hpp>

#include "effects_data.hpp"

#include <string>
#include <vector>

#include <fmt/format.h>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/validation/effects/condition/condition_data.hpp>
#include <core/validation/effects/stat_change/stat_change_data.hpp>
#include <core/validation/effects/subholders/action_holder_data.hpp>
#include <core/validation/effects/subholders/extra_spells_holder_data.hpp>
#include <core/validation/effects/subholders/proficiency_holder_data.hpp>
#include <core/validation/effects/subholders/riv_holder_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

Errors validate_effects_recursively_for_content(const EffectsData& data, const Content& content) {
    Errors errors;
    for (const ConditionData& condition_data : data.activation_conditions_data) {
        errors += validate_condition(condition_data);
    }
    for (const ChoiceData& choice_data : data.choices_data) {
        errors += validate_choice_for_content(choice_data, content);
    }
    for (const StatChangeData& stat_change_data : data.stat_changes_data) {
        errors += validate_stat_change(stat_change_data);
    }
    errors += validate_actions_holder(data.action_holder_data);
    errors += validate_extra_spells_holder_for_content(data.extra_spells_holder_data, content);
    errors += validate_proficiency_holder_for_content(data.proficiency_holder_data, content);
    errors += validate_riv_holder_for_content(data.riv_holder_data, content);
    return errors;
}

} // namespace dnd
