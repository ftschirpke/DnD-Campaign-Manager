#ifndef IMPORTANT_LEVELS_HPP_
#define IMPORTANT_LEVELS_HPP_

#include <dnd_config.hpp>

#include <set>

#include <core/utils/data_result.hpp>

namespace dnd {

class ImportantLevels {
public:
    struct Data;

    static CreateResult<ImportantLevels> create(Data&& data, int subclass_level);

    const std::set<int>& get_feat_levels() const noexcept;
    int get_subclass_level() const noexcept;
private:
    ImportantLevels(std::set<int>&& feat_levels, int subclass_level) noexcept;

    std::set<int> feat_levels;
    int subclass_level;
};

struct ImportantLevels::Data {
    std::strong_ordering operator<=>(const Data&) const noexcept = default;

    std::set<int> feat_levels;
};

} // namespace dnd

#endif // IMPORTANT_LEVELS_HPP_
