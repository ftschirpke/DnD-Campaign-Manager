#ifndef PROGRESSION_DATA_HPP_
#define PROGRESSION_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class ProgressionData : public ValidationSubdata {
public:
    ProgressionData(std::shared_ptr<const ValidationData> parent) noexcept;
    std::strong_ordering operator<=>(const ProgressionData&) const noexcept = default;

    int level;
    int xp;
    std::vector<int> hit_dice_rolls;
};

Errors validate_progression(const ProgressionData& data);

} // namespace dnd

#endif // PROGRESSION_DATA_HPP_
