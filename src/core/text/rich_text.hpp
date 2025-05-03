#ifndef RICH_TEXT_HPP_
#define RICH_TEXT_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <optional>
#include <string>
#include <vector>

namespace dnd {

struct RichAttribute {
    std::strong_ordering operator<=>(const RichAttribute&) const = default;

    std::optional<std::string> key;
    std::string value;
};

struct RichText {
    std::strong_ordering operator<=>(const RichText&) const = default;

    std::string rich_type;
    std::string text;
    std::vector<RichAttribute> attributes;
    size_t length;
};

std::optional<RichText> parse_rich_text(const std::string& str);
std::optional<RichText> parse_rich_text(std::string::const_iterator begin, std::string::const_iterator end);

} // namespace dnd

#endif // RICH_TEXT_HPP_
