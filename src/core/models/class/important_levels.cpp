#include <dnd_config.hpp>

#include "important_levels.hpp"

#include <set>
#include <utility>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/utils/data_result.hpp>
#include <core/validation/class/important_levels_data.hpp>

namespace dnd {

CreateResult<ImportantLevels> ImportantLevels::create(Data&& data, int subclass_level) {
    if (subclass_level <= 0 || subclass_level > 20) {
        throw invalid_data(
            "Cannot create important levels object with a subclass level that is not between 1 and 20 (inclusive)."
        ); // TODO: make this a validation error and maybe integrate it into the ImportantLevelsData validation?!
    }
    Errors errors = validate_important_levels(data);
    if (!errors.ok()) {
        return InvalidCreate<ImportantLevels>(std::move(data), std::move(errors));
    }
    return ValidCreate(ImportantLevels(std::move(data.feat_levels), subclass_level));
}

const std::set<int>& ImportantLevels::get_feat_levels() const noexcept { return feat_levels; }

int ImportantLevels::get_subclass_level() const noexcept { return subclass_level; }

ImportantLevels::ImportantLevels(std::set<int>&& feat_levels, int subclass_level) noexcept
    : feat_levels(std::move(feat_levels)), subclass_level(subclass_level) {}

} // namespace dnd
