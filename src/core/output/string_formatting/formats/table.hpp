#ifndef TABLE_HPP_
#define TABLE_HPP_

#include <dnd_config.hpp>

#include <string_view>
#include <vector>

#include <core/output/string_formatting/format_visitor.hpp>
#include <core/output/string_formatting/formats/format.hpp>

namespace dnd {

class Table : public Format {
public:
    explicit Table();
    /**
     * @brief Accept a format visitor
     * @param visitor a pointer to the format visitor
     */
    virtual void accept(const FormatVisitor& visitor) const override;
    void add_element(std::string_view element);
    void next_row();
    std::vector<std::vector<std::string_view>> get_rows() const;
    size_t get_num_columns() const;
private:
    size_t num_columns;
    size_t current_row;
    std::vector<std::vector<std::string_view>> rows;
};

} // namespace dnd

#endif // TABLE_HPP_
