#ifndef PROGRESSION_DATA_HPP_
#define PROGRESSION_DATA_HPP_

#include <dnd_config.hpp>

#include <vector>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class ProgressionData : public ValidationSubdata {
public:
    ProgressionData(const ValidationData* parent) noexcept;
    /**
     * @brief Validates the data
     * @return the errors that occured during validation
     */
    virtual Errors validate() const override;

    int level;
    int xp;
    std::vector<int> hit_dice_rolls;
};

} // namespace dnd

#endif // PROGRESSION_DATA_HPP_
