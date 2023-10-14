#include <dnd_config.hpp>

#include "table.hpp"

#include <string_view>
#include <vector>

#include <core/output/string_formatting/format_visitor.hpp>

dnd::Table::Table() noexcept : num_columns(0), current_row(0), rows({{}}) {}

void dnd::Table::accept(const FormatVisitor& visitor) const { visitor.visit(*this); }

std::vector<std::vector<std::string_view>> dnd::Table::get_rows() const noexcept { return rows; }

size_t dnd::Table::get_num_columns() const noexcept { return num_columns; }


void dnd::Table::add_element(std::string_view element) {
    rows[current_row].push_back(element);
    if (rows[current_row].size() > num_columns) {
        num_columns = rows[current_row].size();
        for (size_t prev_row = 0; prev_row < current_row; ++prev_row) {
            rows[prev_row].resize(num_columns);
        }
    }
}

void dnd::Table::next_row() {
    rows.push_back({});
    ++current_row;
}
