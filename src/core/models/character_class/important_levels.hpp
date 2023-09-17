#ifndef IMPORTANT_LEVELS_HPP_
#define IMPORTANT_LEVELS_HPP_

#include <dnd_config.hpp>

#include <set>
#include <vector>

#include <core/validation/character_class/important_levels_data.hpp>

namespace dnd {

class ImportantLevels {
public:
    /**
     * @brief Constructs an ImportantLevels object from the given data and subclass level
     * @param data the data to construct the object from
     * @param subclass_level the subclass level
     * @return the constructed object
     * @throw dnd::invalid_data if the data is invalid or the subclass level is not between 1 and 20 (inclusive)
     */
    static ImportantLevels create(ImportantLevelsData&& data, int subclass_level);

    /**
     * @brief Returns the feat levels
     * @return the feat levels
     */
    const std::set<int>& get_feat_levels() const noexcept;
    /**
     * @brief Returns the subclass level
     * @return the subclass level
     */
    int get_subclass_level() const noexcept;
private:
    ImportantLevels(std::set<int>&& feat_levels, int subclass_level) noexcept;

    std::set<int> feat_levels;
    int subclass_level;
};

} // namespace dnd

#endif // IMPORTANT_LEVELS_HPP_
