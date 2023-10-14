#include <dnd_config.hpp>

#include "feature_data.hpp"

#include <memory>

#include <core/errors/errors.hpp>
#include <core/validation/effects/effects_data.hpp>
#include <core/validation/validation_data.hpp>

dnd::FeatureData::FeatureData(const ValidationData* parent) noexcept
    : ValidationData(), main_effects_data(parent == nullptr ? this : parent), parent(parent) {}

std::unique_ptr<dnd::ValidationData> dnd::FeatureData::pack() const { return std::make_unique<FeatureData>(*this); }

dnd::Errors dnd::FeatureData::validate() const {
    Errors errors = ValidationData::validate();
    errors += main_effects_data.validate();
    return errors;
}

dnd::Errors dnd::FeatureData::validate_relations(const Content& content) const {
    return main_effects_data.validate_relations(content);
}

const dnd::ValidationData* dnd::FeatureData::get_parent() const noexcept { return parent; }
