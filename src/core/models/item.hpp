#ifndef ITEM_HPP_
#define ITEM_HPP_

#include "dnd_config.hpp"

#include <string>

#include "models/content_piece.hpp"

namespace dnd {

/**
 * @brief A class representing an item (something a character can equip)
 */
class Item : public ContentPiece {
public:
    /**
     * @brief Constructs an item (without cosmetic description)
     * @param name the name of the item
     * @param requires_attunement whether the item requires attunement
     * @param description a description of how the item works and/or what is does
     */
    Item(
        const std::string& name, const std::filesystem::path& source_file_path, bool requires_attunement,
        const std::string description
    ) noexcept;
    /**
     * @brief Constructs an item with cosmetic description
     * @param name the name of the item
     * @param requires_attunement whether the item requires attunement
     * @param description a description of how the item works and/or what is does
     * @param cosmetic_description a description of the purely cosmetic (non-functional) aspects of the item
     */
    Item(
        const std::string& name, const std::filesystem::path& source_file_path, bool requires_attunement,
        const std::string description, const std::string cosmetic_description
    ) noexcept;

    const std::string name;
    const bool requires_attunement;
    // a functional description of the item (how it works and what it does)
    const std::string description;
    // a description of the item focusing on its purely cosmetic (non-functional) aspects
    const std::string cosmetic_description;
};

inline Item::Item(
    const std::string& name, const std::filesystem::path& source_file_path, bool requires_attunement,
    const std::string description
) noexcept
    : ContentPiece(name, source_file_path), requires_attunement(requires_attunement), description(description),
      cosmetic_description() {}

inline Item::Item(
    const std::string& name, const std::filesystem::path& source_file_path, bool requires_attunement,
    const std::string description, const std::string cosmetic_description
) noexcept
    : ContentPiece(name, source_file_path), requires_attunement(requires_attunement), description(description),
      cosmetic_description(cosmetic_description) {}

} // namespace dnd

#endif // ITEM_HPP_
