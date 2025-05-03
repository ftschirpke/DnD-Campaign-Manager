#ifndef TEXT_HPP_
#define TEXT_HPP_

#include <dnd_config.hpp>

#include <optional>
#include <string>
#include <variant>
#include <vector>

#include <core/text/rich_text.hpp>

namespace dnd {

struct SimpleText {
    std::strong_ordering operator<=>(const SimpleText&) const = default;

    std::string str;
    bool italic = false;
    bool bold = false;
};

struct Link {
    std::strong_ordering operator<=>(const Link&) const = default;

    std::string str;
    std::vector<RichAttribute> attributes;
    bool italic = false;
    bool bold = false;
};

using InlineText = std::variant<SimpleText, Link>;

struct Paragraph {
    std::strong_ordering operator<=>(const Paragraph&) const = default;

    std::vector<InlineText> parts;
};

struct List {
    std::strong_ordering operator<=>(const List&) const = default;

    std::vector<Paragraph> parts;
    std::optional<Paragraph> text_above;
    std::optional<Paragraph> text_below;
};

struct Table {
    std::strong_ordering operator<=>(const Table&) const = default;

    std::optional<std::string> caption;
    size_t columns = 0;
    std::optional<std::vector<std::optional<int>>> column_widths;
    std::vector<std::string> header;
    std::vector<std::vector<Paragraph>> rows;
};

using TextObject = std::variant<Paragraph, List, Table>;

struct Text {
    static Text simple(std::string&& str);
    std::strong_ordering operator<=>(const Text&) const = default;

    std::vector<TextObject> parts;
};

inline Text Text::simple(std::string&& str) {
    return Text{.parts = {Paragraph{.parts = {SimpleText{.str = str, .italic = false, .bold = false}}}}};
}

} // namespace dnd

#endif // TEXT_HPP_
