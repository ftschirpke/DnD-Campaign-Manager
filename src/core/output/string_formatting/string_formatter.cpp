#include "dnd_config.hpp"

#include "string_formatter.hpp"

#include <memory>
#include <string>
#include <vector>

#include "core/output/string_formatting/formats/bulleted_list.hpp"
#include "core/output/string_formatting/formats/format.hpp"
#include "core/output/string_formatting/formats/paragraph.hpp"
#include "core/output/string_formatting/formats/table.hpp"

std::vector<std::unique_ptr<dnd::Format>> dnd::StringFormatter::parse_formats(const std::string& text) const {
    std::vector<std::unique_ptr<dnd::Format>> formats;

    std::string::const_iterator it = text.cbegin();

    while (it != text.end()) {
        switch (*it) {
            case '-':
                formats.emplace_back(parse_bulleted_list(it, text.cend()));
                break;
            case '|':
                formats.emplace_back(parse_table(it, text.cend()));
                break;
            default:
                formats.emplace_back(parse_paragraph(it, text.cend()));
        }
        ++it;
    }
    return formats;
}

std::unique_ptr<dnd::Paragraph> dnd::StringFormatter::parse_paragraph(
    std::string::const_iterator& it, const std::string::const_iterator& end_it
) const {
    std::string::const_iterator start_it = it;
    while (*it != '\n') {
        ++it;
    }
    bool empty_line_after = it + 1 == end_it ? false : *(it + 1) == '\n';

    std::unique_ptr<Paragraph> paragraph(
        new Paragraph(std::string_view(start_it, it), empty_line_after && !ignore_double_newline)
    );

    if (empty_line_after) {
        ++it;
    }
    return paragraph;
}

std::unique_ptr<dnd::BulletedList> dnd::StringFormatter::parse_bulleted_list(
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
            bulleted_list->add_item(std::string_view(last_it, it));
            ++it;
            if (it == end_it || *it != '-') {
                return bulleted_list;
            }
        } else {
            ++it;
        }
    }
    if (last_it != it) {
        bulleted_list->add_item(std::string_view(last_it, it));
    }
    return bulleted_list;
}

std::unique_ptr<dnd::Table> dnd::StringFormatter::parse_table(
    std::string::const_iterator& it, const std::string::const_iterator& end_it
) const {
    std::unique_ptr<Table> table(new Table());

    std::string::const_iterator last_it = ++it;
    while (it != end_it) {
        if (*it == '|') {
            table->add_element(std::string_view(last_it, it));
            last_it = ++it;
        } else if (*it == '\n') {
            ++it;
            if (it == end_it || *it != '|') {
                return table;
            }
            table->next_row();
        } else {
            ++it;
        }
    }
    if (last_it != it) {
        table->add_element(std::string_view(last_it, it));
    }
    return table;
}