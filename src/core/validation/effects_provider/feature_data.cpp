#include <dnd_config.hpp>

#include "feature_data.hpp"

#include <memory>

#include <core/errors/errors.hpp>
#include <core/validation/effects/effects_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

FeatureData::FeatureData(const ValidationData* parent) noexcept
    : ValidationData(), main_effects_data(parent == nullptr ? this : parent), parent(parent) {}

std::unique_ptr<ValidationData> FeatureData::pack() const { return std::make_unique<FeatureData>(*this); }

Errors FeatureData::validate() const {
    Errors errors = ValidationData::validate();
    errors += main_effects_data.validate();
    return errors;
}

Errors FeatureData::validate_relations(const Content& content) const {
    return main_effects_data.validate_relations(content);
}

const ValidationData* FeatureData::get_parent() const noexcept { return parent; }

} // namespace dnd
