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
    virtual std::unique_ptr<ValidationData> pack() const override;

    std::string cosmetic_description;
    bool requires_attunement;
};

} // namespace dnd

#endif // ITEM_DATA_HPP_
