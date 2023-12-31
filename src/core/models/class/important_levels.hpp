#ifndef IMPORTANT_LEVELS_HPP_
#define IMPORTANT_LEVELS_HPP_

#include <dnd_config.hpp>

#include <set>

#include <core/utils/data_result.hpp>
#include <core/validation/class/important_levels_data.hpp>

namespace dnd {

class ImportantLevels {
public:
    using Data = ImportantLevelsData;

    static CreateResult<ImportantLevels> create(Data&& data, int subclass_level);

    const std::set<int>& get_feat_levels() const noexcept;
    int get_subclass_level() const noexcept;
private:
    ImportantLevels(std::set<int>&& feat_levels, int subclass_level) noexcept;

    std::set<int> feat_levels;
    int subclass_level;
};

} // namespace dnd

#endif // IMPORTANT_LEVELS_HPP_
