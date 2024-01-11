#include <dnd_config.hpp>

#include "feature_validation.hpp"

#include <core/errors/errors.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/validation/effects/effects_validation.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

Errors validate_feature_nonrecursively(const Feature::Data& data) { return validate_name_description_and_source(data); }

Errors validate_feature_recursively_for_content(const Feature::Data& data, const Content& content) {
    Errors errors = validate_feature_nonrecursively(data);
    errors += validate_effects_recursively_for_content(data.main_effects_data, content);
    return errors;
}

} // namespace dnd
