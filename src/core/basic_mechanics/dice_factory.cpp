#include <dnd_config.hpp>

#include "dice_factory.hpp"

#include <core/basic_mechanics/dice.hpp>
#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/utils/char_manipulation.hpp>
#include <core/validation/basic_mechanics/dice_data.hpp>

namespace dnd {

Dice create_dice(DiceData&& data) {
    if (!data.validate().ok()) {
        throw invalid_data("Cannot create dice from invalid data.");
    }
    data.str[0] = char_to_lowercase(data.str[0]);
    return string_to_dice(data.str);
}

} // namespace dnd
