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
    explicit ClassFeatureData(const ValidationData* parent = nullptr) noexcept;
    std::strong_ordering operator<=>(const ClassFeatureData&) const noexcept = default;
    virtual std::unique_ptr<ValidationData> pack() const override;
    virtual Errors validate() const override;
    virtual Errors validate_nonrecursively() const override;
    virtual Errors validate_relations(const Content& content) const override;

    int level;
    std::map<int, EffectsData> higher_level_effects_data;
};

} // namespace dnd

#endif // CLASS_FEATURE_DATA_HPP_
