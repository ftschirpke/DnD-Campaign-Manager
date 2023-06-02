#include "dnd_config.hpp"

#include "table.hpp"

#include <string_view>
#include <vector>

void dnd::Table::add_element(std::string_view element) {
    rows[current_row].push_back(element);
    if (rows[current_row] > num_columns) {
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
