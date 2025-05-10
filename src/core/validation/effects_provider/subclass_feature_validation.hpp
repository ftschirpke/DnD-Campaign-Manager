#ifndef SUBCLASS_FEATURE_VALIDATION_HPP_
#define SUBCLASS_FEATURE_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <core/errors/errors.hpp>
#include <core/models/effects_provider/subclass_feature.hpp>

namespace dnd {

Errors validate_subclass_feature_nonrecursively(const SubclassFeature::Data& data);
Errors validate_subclass_feature_recursively_for_content(const SubclassFeature::Data& data, const Content& content);

} // namespace dnd

#endif // SUBCLASS_FEATURE_VALIDATION_HPP_
