#ifndef CHOOSABLE_DATA_HPP_
#define CHOOSABLE_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <memory>
#include <vector>

#include <core/validation/effects/condition/condition_data.hpp>
#include <core/validation/feature/feature_data.hpp>

namespace dnd {

class ChoosableData : public FeatureData {
public:
    ChoosableData() noexcept;
    std::strong_ordering operator<=>(const ChoosableData&) const noexcept = default;
    /**
     * @brief Packs the data into a ValidationData unique pointer
     * @return the packed data
     */
    virtual std::unique_ptr<ValidationData> pack() const override;
    /**
     * @brief Validates the data
     * @return the errors that occured during validation
     */
    virtual Errors validate() const override;

    std::string type;
    std::vector<ConditionData> prerequisites_data;
};

} // namespace dnd

#endif // CHOOSABLE_DATA_HPP_
