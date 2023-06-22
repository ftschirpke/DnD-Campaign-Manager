#include <dnd_config.hpp>

#include "item_data.hpp"

#include <memory>

std::unique_ptr<dnd::ValidationData> dnd::ItemData::pack() const { return std::make_unique<ItemData>(*this); }
