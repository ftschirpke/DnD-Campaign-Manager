#include <dnd_config.hpp>

#include "string_formatter.hpp"

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <core/output/string_formatting/formats/bulleted_list.hpp>
#include <core/output/string_formatting/formats/format.hpp>
#include <core/output/string_formatting/formats/paragraph.hpp>
#include <core/output/string_formatting/formats/table.hpp>
#include <core/utils/string_manipulation.hpp>

namespace dnd {

StringFormatter::StringFormatter(bool ignore_double_newline) noexcept : ignore_double_newline(ignore_double_newline) {}

std::vector<std::unique_ptr<Format>> StringFormatter::parse_formats(const std::string& text) const {
    std::vector<std::unique_ptr<Format>> formats;

    std::string::const_iterator it = text.cbegin();

    while (it != text.end()) {
        // iterator is at the start of a new line
        if (*it == '-') {
            formats.emplace_back(parse_bulleted_list(it, text.cend()));
        } else {
            std::string::const_iterator test_it = it;
            bool is_table = false;
            while (test_it != text.end() && *test_it != '\n') {
                if (*test_it == '|') {
                    is_table = true;
                    break;
                }
                ++test_it;
            }
            if (is_table) {
                formats.emplace_back(parse_table(it, text.cend()));
            } else {
                formats.emplace_back(parse_paragraph(it, text.cend()));
            }
        }
        if (it == text.end()) {
            break;
        }
        ++it;
    }
    return formats;
}

std::unique_ptr<Paragraph> StringFormatter::parse_paragraph(
    std::string::const_iterator& it, const std::string::const_iterator& end_it
) const {
    std::string::const_iterator start_it = it;
    while (it != end_it && *it != '\n') {
        ++it;
    }
    if (it == end_it) {
        return std::make_unique<Paragraph>(str_view(start_it, it), false);
    }

    bool empty_line_after = it + 1 == end_it ? false : *(it + 1) == '\n';

    std::unique_ptr<Paragraph> paragraph(
        new Paragraph(str_view(start_it, it), empty_line_after && !ignore_double_newline)
    );

    if (empty_line_after) {
        ++it;
    }
    return paragraph;
}

std::unique_ptr<BulletedList> StringFormatter::parse_bulleted_list(
    std::string::const_iterator& it, const std::string::const_iterator& end_it
) const {
    std::unique_ptr<BulletedList> bulleted_list(new BulletedList());

    std::string::const_iterator last_it = it;
    while (it != end_it) {
        if (*it == '-') {
            ++it;
            if (it != end_it && *it == ' ') {
                ++it;
            }
            last_it = it;
        } else if (*it == '\n') {
            bulleted_list->add_item(str_view(last_it, it));
            ++it;
            if (it == end_it || *it != '-') {
                return bulleted_list;
            }
        } else {
            ++it;
        }
    }
    if (last_it != it) {
        bulleted_list->add_item(str_view(last_it, it));
    }
    return bulleted_list;
}

std::unique_ptr<Table> StringFormatter::parse_table(
    std::string::const_iterator& it, const std::string::const_iterator& end_it
) const {
    std::unique_ptr<Table> table(new Table());

    std::string::const_iterator last_it = it;
    while (it != end_it) {
        if (*it == '|') {
            table->add_element(str_view(last_it, it));
            last_it = ++it;
        } else if (*it == '\n') {
            table->add_element(str_view(last_it, it));
            last_it = ++it;
            if (it == end_it || *it == '\n') {
                return table;
            }
            table->next_row();
        } else {
            ++it;
        }
    }
    if (last_it != it) {
        table->add_element(str_view(last_it, it));
    }
    return table;
}

} // namespace dnd
