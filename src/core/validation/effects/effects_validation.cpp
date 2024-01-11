#include <dnd_config.hpp>

#include "effects_validation.hpp"

#include <fmt/format.h>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/validation/effects/choice/choice_validation.hpp>
#include <core/validation/effects/condition/condition_validation.hpp>
#include <core/validation/effects/stat_change/stat_change_validation.hpp>
#include <core/validation/effects/subholders/action_holder_validation.hpp>
#include <core/validation/effects/subholders/extra_spells_holder_validation.hpp>
#include <core/validation/effects/subholders/proficiency_holder_validation.hpp>
#include <core/validation/effects/subholders/riv_holder_validation.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

Errors validate_effects_recursively_for_content(const Effects::Data& data, const Content& content) {
    Errors errors;
    for (const Condition::Data& condition_data : data.activation_conditions_data) {
        errors += validate_condition(condition_data);
    }
    for (const Choice::Data& choice_data : data.choices_data) {
        errors += validate_choice_for_content(choice_data, content);
    }
    for (const StatChange::Data& stat_change_data : data.stat_changes_data) {
        errors += validate_stat_change(stat_change_data);
    }
    errors += validate_actions_holder(data.action_holder_data);
    errors += validate_extra_spells_holder_for_content(data.extra_spells_holder_data, content);
    errors += validate_proficiency_holder_for_content(data.proficiency_holder_data, content);
    errors += validate_riv_holder_for_content(data.riv_holder_data, content);
    return errors;
}

} // namespace dnd
