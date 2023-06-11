#include <dnd_config.hpp>

#include "effect_holder_data.hpp"

#include <string>
#include <vector>

#include <fmt/format.h>

#include <core/controllers/content_holder.hpp>
#include <core/errors/errors.hpp>
#include <core/validation/effect_holder/activation/activation_data.hpp>
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
    errors.merge(action_holder_data.validate());
    errors.merge(extra_spells_holder_data.validate());
    errors.merge(proficiency_holder_data.validate());
    errors.merge(riv_holder_data.validate());
    if (activations.empty() && effects.empty() && action_holder_data.empty() && extra_spells_holder_data.empty()
        && proficiency_holder_data.empty() && riv_holder_data.empty()) {
        errors.add_validation_error(
            ValidationErrorCode::MISSING_ATTRIBUTE, parent, "Effect holder is completely empty."
        );
    }
    return errors;
}

dnd::Errors dnd::EffectHolderData::validate_relations(const ContentHolder* content) const {
    Errors errors;
    errors.merge(extra_spells_holder_data.validate_relations(content));
    errors.merge(proficiency_holder_data.validate_relations(content));
    errors.merge(riv_holder_data.validate_relations(content));
    return errors;
}
