#ifndef FEATURE_VALIDATION_HPP_
#define FEATURE_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <core/errors/errors.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/validation/effects/effects_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class Content;

Errors validate_feature_nonrecursively(const Feature::Data& data);
Errors validate_feature_recursively_for_content(const Feature::Data& data, const Content& content);

} // namespace dnd

#endif // FEATURE_VALIDATION_HPP_
