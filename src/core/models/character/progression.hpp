#ifndef PROGRESSION_HPP_
#define PROGRESSION_HPP_

#include <dnd_config.hpp>

#include <vector>

#include <core/utils/data_result.hpp>
#include <core/validation/character/progression_data.hpp>

namespace dnd {

class Content;

class Progression {
public:
    using Data = ProgressionData;

    static CreateResult<Progression> create(Data&& data);

    int get_level() const noexcept;
    int get_xp() const noexcept;
    const std::vector<int>& get_hit_dice_rolls() const noexcept;
private:
    Progression(int level, int xp, std::vector<int>&& hit_dice_rolls) noexcept;

    int level;
    int xp;
    std::vector<int> hit_dice_rolls;
};

} // namespace dnd

#endif // PROGRESSION_HPP_
