#ifndef TABLE_HPP_
#define TABLE_HPP_

#include "dnd_config.hpp"

#include <string_view>
#include <vector>

#include "core/output/string_formatting/formats/format.hpp"

namespace dnd {

class Table : public Format {
public:
    explicit Table() noexcept;
    /**
     * @brief Accept a format visitor
     * @param visitor a pointer to the format visitor
     */
    virtual void accept(FormatVisitor* visitor);
    /**
     * @brief Add an element to the current row
     * @param element the element to add
     */
    void add_element(std::string_view element);
    /**
     * @brief End the current row and start a new one
     */
    void next_row();
private:
    size_t num_columns;
    size_t current_row;
    std::vector<std::vector<std::string_view>> rows;
};

inline Table::Table() noexcept : num_columns(0), current_row(0), rows({{}}) {}

inline void Table::accept(FormatVisitor* visitor) { visitor->visit(this); }

} // namespace dnd

#endif // TABLE_HPP_
