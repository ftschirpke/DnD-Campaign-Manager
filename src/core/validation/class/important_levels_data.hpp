#ifndef IMPORTANT_LEVELS_DATA_HPP_
#define IMPORTANT_LEVELS_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <set>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class ImportantLevelsData : public ValidationSubdata {
public:
    ImportantLevelsData(std::shared_ptr<const ValidationData> parent) noexcept;
    std::strong_ordering operator<=>(const ImportantLevelsData&) const noexcept = default;

    std::set<int> feat_levels;
};

Errors validate_important_levels(const ImportantLevelsData& data);

} // namespace dnd

#endif // IMPORTANT_LEVELS_DATA_HPP_
