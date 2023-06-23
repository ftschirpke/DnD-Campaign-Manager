#include <dnd_config.hpp>

#include "important_levels.hpp"

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
    return ImportantLevels(std::move(data.asi_levels), subclass_level);
}

const std::vector<int>& dnd::ImportantLevels::get_asi_levels() const noexcept { return asi_levels; }

int dnd::ImportantLevels::get_subclass_level() const noexcept { return subclass_level; }

dnd::ImportantLevels::ImportantLevels(std::vector<int>&& asi_levels, int subclass_level) noexcept
    : asi_levels(std::move(asi_levels)), subclass_level(subclass_level) {}
