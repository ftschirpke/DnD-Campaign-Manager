#ifndef PROGRESSION_DATA_HPP_
#define PROGRESSION_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <vector>

#include <core/errors/errors.hpp>

namespace dnd {

class ProgressionData {
public:
    std::strong_ordering operator<=>(const ProgressionData&) const noexcept = default;

    int level;
    int xp;
    std::vector<int> hit_dice_rolls;
};

Errors validate_progression(const ProgressionData& data);

} // namespace dnd

#endif // PROGRESSION_DATA_HPP_
