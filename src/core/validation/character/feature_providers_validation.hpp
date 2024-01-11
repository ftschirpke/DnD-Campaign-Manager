#ifndef FEATURE_PROVIDERS_VALIDATION_HPP_
#define FEATURE_PROVIDERS_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <core/errors/errors.hpp>
#include <core/models/character/feature_providers.hpp>

namespace dnd {

class Content;

Errors validate_feature_providers_for_content(const FeatureProviders::Data& data, const Content& content);

} // namespace dnd

#endif // FEATURE_PROVIDERS_VALIDATION_HPP_
