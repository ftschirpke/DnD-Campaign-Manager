#ifndef DICE_DATA_HPP_
#define DICE_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <string>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class DiceData : public ValidationSubdata {
public:
    DiceData(const ValidationData* parent) noexcept;
    std::strong_ordering operator<=>(const DiceData&) const noexcept = default;
    virtual Errors validate() const override;

    std::string str;
};

} // namespace dnd

#endif // DICE_DATA_HPP_
