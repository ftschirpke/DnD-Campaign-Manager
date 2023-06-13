#ifndef DICE_DATA_HPP_
#define DICE_DATA_HPP_

#include <dnd_config.hpp>

#include <string>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class DiceData : public ValidationSubdata {
public:
    DiceData(const ValidationData* parent) noexcept;
    /**
     * @brief Validates the data
     * @return the errors that occured during validation
     */
    virtual Errors validate() const override;

    std::string str;
};

} // namespace dnd

#endif // DICE_DATA_HPP_
