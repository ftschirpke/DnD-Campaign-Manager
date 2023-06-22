#include <dnd_config.hpp>

#include "effect_holder_data.hpp"

#include <string>
#include <vector>

#include <fmt/format.h>

#include <core/controllers/content_holder.hpp>
#include <core/errors/errors.hpp>
#include <core/validation/effect_holder/condition/condition_data.hpp>
#include <core/validation/effect_holder/effect/effect_data.hpp>
#include <core/validation/effect_holder/subholders/action_holder_data.hpp>
#include <core/validation/effect_holder/subholders/extra_spells_holder_data.hpp>
#include <core/validation/effect_holder/subholders/proficiency_holder_data.hpp>
#include <core/validation/effect_holder/subholders/riv_holder_data.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

dnd::EffectHolderData::EffectHolderData(const ValidationData* parent) noexcept
    : ValidationSubdata(parent), action_holder_data(parent), extra_spells_holder_data(parent),
      proficiency_holder_data(parent), riv_holder_data(parent) {}

dnd::Errors dnd::EffectHolderData::validate() const {
    Errors errors;
    for (const auto& condition_data : activation_conditions_data) {
        errors += condition_data.validate();
    }
    for (const auto& choice_data : choices_data) {
        errors += choice_data.validate();
    }
    for (const auto& effect_data : effects_data) {
        errors += effect_data.validate();
    }
    errors += action_holder_data.validate();
    errors += extra_spells_holder_data.validate();
    errors += proficiency_holder_data.validate();
    errors += riv_holder_data.validate();
    return errors;
}

dnd::Errors dnd::EffectHolderData::validate_relations(const ContentHolder& content) const {
    Errors errors;
    for (const auto& choice_data : choices_data) {
        errors += choice_data.validate_relations(content);
    }
    errors += extra_spells_holder_data.validate_relations(content);
    errors += proficiency_holder_data.validate_relations(content);
    errors += riv_holder_data.validate_relations(content);
    return errors;
}
