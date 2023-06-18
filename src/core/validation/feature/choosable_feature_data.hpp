#ifndef CHOOSABLE_FEATURE_DATA_HPP_
#define CHOOSABLE_FEATURE_DATA_HPP_

#include <dnd_config.hpp>

#include <vector>

#include <core/validation/effect_holder/condition/condition_data.hpp>
#include <core/validation/feature/feature_data.hpp>

namespace dnd {

class ChoosableFeatureData : public FeatureData {
public:
    ChoosableFeatureData() noexcept;
    /**
     * @brief Validates the data
     * @return the errors that occured during validation
     */
    virtual Errors validate() const override;

    std::string type;
    std::vector<ConditionData> prerequisites_data;
};

} // namespace dnd

#endif // CHOOSABLE_FEATURE_DATA_HPP_