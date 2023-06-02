#ifndef BULLETED_LIST_HPP_
#define BULLETED_LIST_HPP_

#include "dnd_config.hpp"

#include <string_view>
#include <vector>

#include "core/output/string_formatting/formats/format.hpp"

namespace dnd {

class BulletedList : public Format {
public:
    explicit BulletedList() noexcept;
    /**
     * @brief Accept a format visitor
     * @param visitor a pointer to the format visitor
     */
    virtual void accept(FormatVisitor* visitor) const;
    /**
     * @brief Add an item to the list
     * @param item the item to add
     */
    void add_item(std::string_view item);
private:
    std::vector<std::string_view> items;
};

inline BulletedList::BulletedList() noexcept : items({}) {}

inline void BulletedList::accept(FormatVisitor* visitor) const { visitor->visit(this); }

inline void BulletedList::add_item(std::string_view item) { items.push_back(item); }

} // namespace dnd

#endif // BULLETED_LIST_HPP_
