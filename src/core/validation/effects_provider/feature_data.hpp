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

class FeatureData : public ValidationData, public ValidationSubdata {
public:
    explicit FeatureData(std::shared_ptr<const ValidationData> parent = nullptr) noexcept;
    std::strong_ordering operator<=>(const FeatureData&) const noexcept = default;
    virtual std::unique_ptr<ValidationData> pack() const override;

    EffectsData main_effects_data;
};

Errors validate_feature_nonrecursively(const FeatureData& data);
Errors validate_feature_recursively_for_content(const FeatureData& data, const Content& content);

} // namespace dnd

#endif // FEATURE_DATA_HPP_
