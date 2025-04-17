#include <dnd_config.hpp>

#include "rich_text.hpp"

#include <cassert>
#include <optional>
#include <string>
#include <vector>

namespace dnd {

enum class Status {
    SEARCHING_START,
    SEARCHING_TYPE_START,
    SEARCHING_TYPE,
    SEARCHING_TEXT,
    SEARCHING_ATTRIBUTES,
    FINISHED,
};

std::optional<RichText> parse_rich_text(const std::string& str) { return parse_rich_text(str.begin(), str.end()); }

std::optional<RichText> parse_rich_text(std::string::const_iterator begin, std::string::const_iterator end) {
    RichText rich_text;
    Status status = Status::SEARCHING_START;

    bool found_start = false;
    std::string::const_iterator start = begin;

    std::optional<std::string> parsed_key = std::nullopt;

    for (std::string::const_iterator str = begin; str != end; str++) {
        switch (status) {
            case Status::SEARCHING_START:
                assert(*str == '{');
                status = Status::SEARCHING_TYPE_START;
                break;
            case Status::SEARCHING_TYPE_START:
                assert(*str == '@');
                status = Status::SEARCHING_TYPE;
                break;
            case Status::SEARCHING_TYPE:
                if (!found_start) {
                    start = str;
                    found_start = true;
                } else if (*str == ' ' || *str == '}') {
                    rich_text.rich_type = std::string(start, str);
                    found_start = false;
                    status = Status::SEARCHING_TEXT;
                    if (*str == '}') {
                        return std::nullopt;
                    }
                }
                break;
            case Status::SEARCHING_TEXT:
                if (!found_start && *str != ' ') {
                    start = str;
                    found_start = true;
                } else if (*str == '|' || *str == '}') {
                    rich_text.text = std::string(start, str);
                    found_start = false;
                    status = Status::SEARCHING_ATTRIBUTES;
                    if (*str == '}') {
                        rich_text.length = str - begin + 1;
                        status = Status::FINISHED;
                    }
                }
                break;
            case Status::SEARCHING_ATTRIBUTES:
                if (!found_start && *str != ' ') {
                    start = str;
                    found_start = true;
                } else if (*str == '=') {
                    parsed_key = std::string(start, str);
                    found_start = false;
                } else if (*str == '|' || *str == '}') {
                    rich_text.attributes.push_back({.key = parsed_key, .value = std::string(start, str)});
                    found_start = false;
                    parsed_key = std::nullopt;
                    if (*str == '}') {
                        rich_text.length = str - begin + 1;
                        status = Status::FINISHED;
                    }
                }
                break;
            case Status::FINISHED:
                return rich_text;
                break;
        }
    }

    if (status != Status::FINISHED) {
        return std::nullopt;
    }
    return rich_text;
}

} // namespace dnd
