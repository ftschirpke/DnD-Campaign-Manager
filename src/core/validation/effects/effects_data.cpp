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
#include <core/validation/validation_subdata.hpp>

dnd::EffectsData::EffectsData(const ValidationData* parent) noexcept
    : ValidationSubdata(parent), activation_conditions_data(), choices_data(), stat_changes_data(),
      action_holder_data(parent), extra_spells_holder_data(parent), proficiency_holder_data(parent),
      riv_holder_data(parent) {}

dnd::Errors dnd::EffectsData::validate() const {
    Errors errors;
    for (const ConditionData& condition_data : activation_conditions_data) {
        errors += condition_data.validate();
    }
    for (const ChoiceData& choice_data : choices_data) {
        errors += choice_data.validate();
    }
    for (const StatChangeData& stat_change_data : stat_changes_data) {
        errors += stat_change_data.validate();
    }
    errors += action_holder_data.validate();
    errors += extra_spells_holder_data.validate();
    errors += proficiency_holder_data.validate();
    errors += riv_holder_data.validate();
    return errors;
}

dnd::Errors dnd::EffectsData::validate_relations(const Content& content) const {
    Errors errors;
    for (const ChoiceData& choice_data : choices_data) {
        errors += choice_data.validate_relations(content);
    }
    errors += extra_spells_holder_data.validate_relations(content);
    errors += proficiency_holder_data.validate_relations(content);
    errors += riv_holder_data.validate_relations(content);
    return errors;
}
