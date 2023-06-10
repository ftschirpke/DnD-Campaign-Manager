#ifndef ITEM_DATA_HPP_
#define ITEM_DATA_HPP_

#include <dnd_config.hpp>

#include <string>

#include <core/validation/validation_data.hpp>

namespace dnd {

class ItemData : public ValidationData {
public:
    std::string cosmetic_description;
    bool requires_attunement;
};

} // namespace dnd

#endif // ITEM_DATA_HPP_
