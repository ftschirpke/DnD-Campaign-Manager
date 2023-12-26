#include <dnd_config.hpp>

#include "display_format_visitor.hpp"

#include <algorithm>
#include <string_view>
#include <vector>

#include <cli/output/command_line_output.hpp>
#include <core/output/string_formatting/formats/bulleted_list.hpp>
#include <core/output/string_formatting/formats/paragraph.hpp>
#include <core/output/string_formatting/formats/table.hpp>

namespace dnd {


void DisplayFormatVisitor::operator()(const BulletedList& bulleted_list) const {
    for (const std::string_view& item : bulleted_list.get_items()) {
        output.formatted_text("- {}", item);
    }
}

void DisplayFormatVisitor::operator()(const Paragraph& paragraph) const {
    output.text(paragraph.get_text());
    if (paragraph.get_empty_line_after()) {
        output.newline();
    }
}

void DisplayFormatVisitor::operator()(const Table& table) const {
    size_t rows = table.get_rows().size();
    size_t cols = table.get_num_columns();
    std::vector<size_t> col_widths(cols, 0);
    for (const std::vector<std::string_view>& row : table.get_rows()) {
        for (size_t i = 0; i < cols; ++i) {
            col_widths[i] = std::max(col_widths[i], row[i].size());
        }
    }
    std::vector<std::vector<std::string>> right_padded_strings(rows);
    // left align first row
    right_padded_strings[0].reserve(cols);
    for (size_t i = 0; i < cols; ++i) {
        std::string_view str = table.get_rows()[0][i];
        right_padded_strings[0].emplace_back(str);
        if (str.size() < col_widths[i]) {
            right_padded_strings[0][i].insert(str.size(), col_widths[i] - str.size(), ' ');
        }
    }
    // right align remaining rows
    for (size_t i = 1; i < rows; ++i) {
        right_padded_strings[i].reserve(cols);
        for (size_t j = 0; j < cols; ++j) {
            std::string_view str = table.get_rows()[i][j];
            right_padded_strings[i].emplace_back(str);
            if (str.size() < col_widths[j]) {
                right_padded_strings[i][j].insert(0, col_widths[j] - str.size(), ' ');
            }
        }
    }
    // print table
    bool first = true;
    for (const std::vector<std::string>& row : right_padded_strings) {
        output.formatted_text(" {} ", fmt::join(row, " | "));
        if (first) {
            // print separator line after first row
            std::vector<std::string> separator_line(cols);
            for (size_t i = 0; i < cols; ++i) {
                separator_line[i] = std::string(col_widths[i], '-');
            }
            output.formatted_text("-{}-", fmt::join(separator_line, "-|-"));
            first = false;
        }
    }
}

} // namespace dnd
