#ifndef IMPORTANT_LEVELS_DATA_HPP_
#define IMPORTANT_LEVELS_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <set>

#include <core/errors/errors.hpp>

namespace dnd {

class ImportantLevelsData {
public:
    std::strong_ordering operator<=>(const ImportantLevelsData&) const noexcept = default;

    std::set<int> feat_levels;
};

Errors validate_important_levels(const ImportantLevelsData& data);

} // namespace dnd

#endif // IMPORTANT_LEVELS_DATA_HPP_
