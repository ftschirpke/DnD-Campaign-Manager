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
    ImportantLevelsData(const ValidationData* parent) noexcept;
    std::strong_ordering operator<=>(const ImportantLevelsData&) const noexcept = default;
    /**
     * @brief Validates the data
     * @return the errors that occured during validation
     */
    virtual Errors validate() const override;

    std::set<int> feat_levels;
};

} // namespace dnd

#endif // IMPORTANT_LEVELS_DATA_HPP_
