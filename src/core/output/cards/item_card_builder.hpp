#ifndef ITEM_CARD_BUILDER_HPP_
#define ITEM_CARD_BUILDER_HPP_

#include <dnd_config.hpp>

#include <vector>

#include <core/models/item/item.hpp>
#include <core/types.hpp>

namespace dnd {

class Content;

class ItemCardBuilder {
public:
    explicit ItemCardBuilder(const Content& content) noexcept;

    void add_item(Id item_id);
    std::vector<Id> get_items() const;
    void clear_items();
    /**
     * @brief Creates a LaTeX file that allows printing the cards with the timestamp as the file name
     */
    std::string write_latex_file();
    /**
     * @brief Creates a LaTeX file that allows printing the cards with a given file name
     * @param filename the name of the file
     */
    void write_latex_file(const std::string& filename);
private:
    const Content& content;
    std::vector<Id> items;
};

} // namespace dnd

#endif // ITEM_CARD_BUILDER_HPP_
