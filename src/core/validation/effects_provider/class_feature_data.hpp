#ifndef CLASS_FEATURE_VALIDATION_HPP_
#define CLASS_FEATURE_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <core/errors/errors.hpp>
#include <core/models/effects_provider/class_feature.hpp>

namespace dnd {

Errors validate_class_feature_nonrecursively(const ClassFeature::Data& data);
Errors validate_class_feature_recursively_for_content(const ClassFeature::Data& data, const Content& content);

} // namespace dnd

#endif // CLASS_FEATURE_VALIDATION_HPP_
