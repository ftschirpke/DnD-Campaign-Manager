#ifndef PROGRESSION_HPP_
#define PROGRESSION_HPP_

#include <dnd_config.hpp>

#include <vector>

#include <core/validation/character/progression_data.hpp>

namespace dnd {

class ContentHolder;

class Progression {
public:
    /**
     * @brief Constructs a Progression object
     * @param data the data to construct the Progression object from
     * @return the constructed Progression object
     * @throws dnd::invalid_data if the given data is invalid
     */
    static Progression create(ProgressionData&& data);
private:
    Progression(int level, int xp, std::vector<int>&& hit_dice_rolls) noexcept;

    int level;
    int xp;
    std::vector<int> hit_dice_rolls;
};

} // namespace dnd

#endif // PROGRESSION_HPP_
