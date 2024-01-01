#ifndef ITEM_DATA_HPP_
#define ITEM_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <memory>
#include <string>

#include <core/validation/validation_data.hpp>

namespace dnd {

class ItemData : public ValidationData {
public:
    std::strong_ordering operator<=>(const ItemData&) const noexcept = default;

    std::string cosmetic_description;
    bool requires_attunement;
};

Errors validate_item(const ItemData& data);

} // namespace dnd

#endif // ITEM_DATA_HPP_
