#ifndef CHOOSABLE_DATA_HPP_
#define CHOOSABLE_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <memory>
#include <vector>

#include <core/validation/effects/condition/condition_data.hpp>
#include <core/validation/effects_provider/feature_data.hpp>

namespace dnd {

class ChoosableData : public FeatureData {
public:
    ChoosableData() noexcept;
    std::strong_ordering operator<=>(const ChoosableData&) const noexcept = default;
    virtual std::unique_ptr<ValidationData> pack() const override;

    std::string type;
    std::vector<ConditionData> prerequisites_data;
};

} // namespace dnd

#endif // CHOOSABLE_DATA_HPP_
