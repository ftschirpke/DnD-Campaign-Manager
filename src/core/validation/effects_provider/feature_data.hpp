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

class FeatureData : public ValidationData {
public:
    explicit FeatureData(const ValidationData* parent = nullptr) noexcept;
    std::strong_ordering operator<=>(const FeatureData&) const noexcept = default;
    virtual std::unique_ptr<ValidationData> pack() const override;
    virtual Errors validate() const override;
    virtual Errors validate_nonrecursively() const;
    virtual Errors validate_relations(const Content& content) const override;

    const ValidationData* get_parent() const noexcept;

    EffectsData main_effects_data;
private:
    const ValidationData* parent;
};

} // namespace dnd

#endif // FEATURE_DATA_HPP_
