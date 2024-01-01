#ifndef CLASS_FEATURE_DATA_HPP_
#define CLASS_FEATURE_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <map>
#include <memory>

#include <core/errors/errors.hpp>
#include <core/validation/effects/effects_data.hpp>
#include <core/validation/effects_provider/feature_data.hpp>

namespace dnd {

class ClassFeatureData : public FeatureData {
public:
    explicit ClassFeatureData(std::shared_ptr<const ValidationData> parent = nullptr) noexcept;
    std::strong_ordering operator<=>(const ClassFeatureData&) const noexcept = default;
    virtual std::unique_ptr<ValidationData> pack() const override;

    int level;
    std::map<int, EffectsData> higher_level_effects_data;
};

Errors validate_class_feature_nonrecursively(const ClassFeatureData& data);
Errors validate_class_feature_recursively_for_content(const ClassFeatureData& data, const Content& content);

} // namespace dnd

#endif // CLASS_FEATURE_DATA_HPP_
