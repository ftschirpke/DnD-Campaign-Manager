#ifndef IMPORTANT_LEVELS_DATA_HPP_
#define IMPORTANT_LEVELS_DATA_HPP_

#include <dnd_config.hpp>

#include <vector>

#include <core/errors/errors.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class CharacterClassData;

class ImportantLevelsData : public ValidationSubdata {
public:
    ImportantLevelsData(const CharacterClassData* parent) noexcept;
    int operator<=>(const ImportantLevelsData&) const noexcept = default;
    /**
     * @brief Validates the data
     * @return the errors that occured during validation
     */
    virtual Errors validate() const override;

    std::vector<int> asi_levels;
};

} // namespace dnd

#endif // IMPORTANT_LEVELS_DATA_HPP_
