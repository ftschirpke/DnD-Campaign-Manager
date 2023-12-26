#include <dnd_config.hpp>

#include "important_levels.hpp"

#include <algorithm>
#include <set>
#include <utility>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>

dnd::ImportantLevels dnd::ImportantLevels::create(dnd::ImportantLevelsData&& data, int subclass_level) {
    if (subclass_level <= 0 || subclass_level > 20) {
        throw dnd::invalid_data(
            "Cannot create important levels object with a subclass level that is not between 1 and 20 (inclusive)."
        );
    }
    if (!data.validate().ok()) {
        throw dnd::invalid_data("Cannot create important levels object from invalid data.");
    }
    return ImportantLevels(std::move(data.feat_levels), subclass_level);
}

const std::set<int>& dnd::ImportantLevels::get_feat_levels() const noexcept { return feat_levels; }

int dnd::ImportantLevels::get_subclass_level() const noexcept { return subclass_level; }

dnd::ImportantLevels::ImportantLevels(std::set<int>&& feat_levels, int subclass_level) noexcept
    : feat_levels(std::move(feat_levels)), subclass_level(subclass_level) {}
