#include <dnd_config.hpp>

#include "feature_data.hpp"

#include <memory>

#include <core/errors/errors.hpp>
#include <core/validation/effects/effects_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

FeatureData::FeatureData(std::shared_ptr<const ValidationData> parent) noexcept
    : ValidationData(), ValidationSubdata(parent), main_effects_data(parent) {}

std::unique_ptr<ValidationData> FeatureData::pack() const { return std::make_unique<FeatureData>(*this); }

Errors validate_feature_nonrecursively(const FeatureData& data) { return validate_name_description_and_source(data); }

Errors validate_feature_for_content_recursively(const FeatureData& data, const Content& content) {
    Errors errors = validate_feature_nonrecursively(data);
    errors += validate_effects_for_content_recursively(data.main_effects_data, content);
    return errors;
}

} // namespace dnd
