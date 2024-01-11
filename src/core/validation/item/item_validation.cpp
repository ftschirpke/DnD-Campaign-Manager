#include <dnd_config.hpp>

#include "item_validation.hpp"

#include <core/errors/errors.hpp>
#include <core/models/item/item.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

Errors validate_item(const Item::Data& data) { return validate_name_description_and_source(data); }

} // namespace dnd
