#ifndef ITEM_HPP_
#define ITEM_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <string>

#include <core/models/content_piece.hpp>
#include <core/models/source_info.hpp>
#include <core/validation/item/item_data.hpp>

namespace dnd {

class ContentVisitor;

class Item : public ContentPiece {
public:
    /**
     * @brief Constructs an item
     * @param item_data the data to construct the item from
     * @return the constructed item
     * @throws dnd::invalid_data if the data is invalid
     */
    static Item create(ItemData&& item_data);

    const std::string& get_name() const noexcept override;
    const std::string& get_description() const noexcept override;
    const SourceInfo& get_source_info() const noexcept override;
    const std::string& get_cosmetic_description() const noexcept;
    bool requires_attunement() const noexcept;

    virtual void accept(ContentVisitor& visitor) const override final;
private:
    Item(
        std::string&& name, std::string&& description, std::filesystem::path&& source_path,
        std::string&& cosmetic_description, bool requires_attunement
    ) noexcept;

    std::string name;
    std::string description;
    SourceInfo source_info;
    std::string cosmetic_description;
    bool attunement;
};

} // namespace dnd

#endif // ITEM_HPP_
