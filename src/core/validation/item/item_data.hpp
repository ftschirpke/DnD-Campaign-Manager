#ifndef ITEM_VALIDATION_HPP_
#define ITEM_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <core/errors/errors.hpp>
#include <core/models/item/item.hpp>

namespace dnd {

Errors validate_item(const Item::Data& data);

} // namespace dnd

#endif // ITEM_VALIDATION_HPP_
