#ifndef FEATURE_DATA_HPP_
#define FEATURE_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <map>
#include <memory>

#include <core/errors/errors.hpp>
#include <core/validation/effects/effects_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class Content;

class FeatureData : public ValidationData {
public:
    std::strong_ordering operator<=>(const FeatureData&) const noexcept = default;

    EffectsData main_effects_data;
};

Errors validate_feature_nonrecursively(const FeatureData& data);
Errors validate_feature_recursively_for_content(const FeatureData& data, const Content& content);

} // namespace dnd

#endif // FEATURE_DATA_HPP_
