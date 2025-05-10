#include <dnd_config.hpp>

#include "latex_rich_text.hpp"

#include <algorithm>
#include <string>

#include <core/text/rich_text.hpp>
#include <log.hpp>

namespace dnd {

std::string derich_text(const std::string& rich_text) {
    std::string deriched_text;
    std::string::const_iterator current = rich_text.cbegin();
    std::string::const_iterator end = rich_text.cend();
    while (current != end) {
        {
            std::string::const_iterator next_rich_text = std::find(current, end, '{');
            if (next_rich_text != current) {
                deriched_text += std::string(current, next_rich_text);
                current = next_rich_text;
            }
        }
        if (current == end) {
            break;
        }
        std::optional<RichText> parsed_rich_text = parse_rich_text(current, end);
        if (parsed_rich_text.has_value()) {
            LOGINFO("Found rich text of type '{}'", parsed_rich_text->rich_type);
            deriched_text += parsed_rich_text->text;
            current += parsed_rich_text->length;
        } else {
            deriched_text += '{';
            current++;
        }
    }

    return deriched_text;
}

} // namespace dnd
