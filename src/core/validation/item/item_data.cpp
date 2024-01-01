#include <dnd_config.hpp>

#include "item_data.hpp"

#include <memory>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

Errors validate_item(const ItemData& data) { return validate_name_description_and_source(data); }

} // namespace dnd
