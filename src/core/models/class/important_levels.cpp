#include <dnd_config.hpp>

#include "important_levels.hpp"

#include <algorithm>
#include <set>
#include <utility>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>

namespace dnd {

ImportantLevels ImportantLevels::create(ImportantLevelsData&& data, int subclass_level) {
    if (subclass_level <= 0 || subclass_level > 20) {
        throw invalid_data(
            "Cannot create important levels object with a subclass level that is not between 1 and 20 (inclusive)."
        );
    }
    if (!data.validate().ok()) {
        throw invalid_data("Cannot create important levels object from invalid data.");
    }
    return ImportantLevels(std::move(data.feat_levels), subclass_level);
}

const std::set<int>& ImportantLevels::get_feat_levels() const noexcept { return feat_levels; }

int ImportantLevels::get_subclass_level() const noexcept { return subclass_level; }

ImportantLevels::ImportantLevels(std::set<int>&& feat_levels, int subclass_level) noexcept
    : feat_levels(std::move(feat_levels)), subclass_level(subclass_level) {}

} // namespace dnd
