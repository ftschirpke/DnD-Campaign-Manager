#ifndef ITEM_CARD_BUILDER_HPP_
#define ITEM_CARD_BUILDER_HPP_

#include "dnd_config.hpp"

#include <vector>

#include "models/item.hpp"

namespace dnd {

class ItemCardBuilder {
public:
    /**
     * @brief Add to the list of items we want to create cards for
     * @param item an item to create a card for
     */
    void addItem(const Item* item);
    /**
     * @brief Creates a LaTeX file that allows printing the cards with the timestamp as the file name
     */
    void writeLatexFile();
    /**
     * @brief Creates a LaTeX file that allows printing the cards with a given file name
     * @param filename the name of the file
     */
    void writeLatexFile(const std::string& filename);
private:
    std::vector<const Item*> items;
};

} // namespace dnd

#endif // ITEM_CARD_BUILDER_HPP_
