#include <dnd_config.hpp>

#include "feature_data.hpp"

#include <memory>

#include <core/errors/errors.hpp>
#include <core/validation/effects/effects_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

Errors validate_feature_nonrecursively(const FeatureData& data) { return validate_name_description_and_source(data); }

Errors validate_feature_recursively_for_content(const FeatureData& data, const Content& content) {
    Errors errors = validate_feature_nonrecursively(data);
    errors += validate_effects_recursively_for_content(data.main_effects_data, content);
    return errors;
}

} // namespace dnd
