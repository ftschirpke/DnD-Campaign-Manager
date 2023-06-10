#ifndef ITEM_HPP_
#define ITEM_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <string>

#include <core/models/content_piece.hpp>


namespace dnd {

class Item : public ContentPiece {
public:
    /**
     * @brief Constructs an item
     * @param name the name of the item
     * @param source_path the path to the source file
     * @param requires_attunement whether the item requires attunement
     * @param description a description of how the item works and/or what is does
     * @param cosmetic_description (optional) a description of the purely cosmetic (non-functional) aspects of the item
     * @return the constructed item
     * @throws dnd::invalid_data if the data is invalid
     */
    static Item create(
        const std::string& name, const std::filesystem::path& source_path, bool requires_attunement,
        const std::string& description, const std::string& cosmetic_description = ""
    );
    /**
     * @brief Constructs an item
     * @param item_data the data to construct the item from
     * @return the constructed item
     * @throws dnd::invalid_data if the data is invalid
     */
    static Item create(ItemData&& item_data);

    /**
     * @brief Accepts a visitor
     * @param visitor pointer to the visitor
     */
    virtual void accept(ContentVisitor* visitor) const override final;
protected:
private:
    Item(
        std::string&& name, std::filesystem::path&& source_path, bool requires_attunement, std::string&& description,
        std::string&& cosmetic_description
    ) noexcept;

    std::string cosmetic_description;
    bool requires_attunement;
};

} // namespace dnd

#endif // ITEM_HPP_
