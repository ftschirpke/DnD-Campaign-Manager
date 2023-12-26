#include <dnd_config.hpp>

#include "item_data.hpp"

#include <memory>

namespace dnd {

std::unique_ptr<ValidationData> ItemData::pack() const { return std::make_unique<ItemData>(*this); }

} // namespace dnd
