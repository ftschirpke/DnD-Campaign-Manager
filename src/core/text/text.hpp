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
    bool bold = false;
    bool italic = false;
};

struct Link {
    std::strong_ordering operator<=>(const Link&) const = default;

    std::string str;
    std::vector<RichAttribute> attributes;
    bool bold = false;
    bool italic = false;
};

using InlineText = std::variant<SimpleText, Link>;

struct Paragraph {
    std::strong_ordering operator<=>(const Paragraph&) const = default;

    std::vector<InlineText> parts;
};

struct Table {
    std::strong_ordering operator<=>(const Table&) const = default;

    std::optional<std::string> caption;
    size_t columns = 0;
    std::optional<std::vector<std::optional<int>>> column_widths;
    std::vector<std::string> header;
    std::vector<std::vector<Paragraph>> rows;
};

struct ListItem {
    std::strong_ordering operator<=>(const ListItem&) const = default;

    std::vector<std::variant<Paragraph, Table>> parts;
};

struct List {
    std::strong_ordering operator<=>(const List&) const = default;

    std::vector<ListItem> parts;
    std::optional<Paragraph> text_above;
    std::optional<Paragraph> text_below;
};

using TextObject = std::variant<Paragraph, List, Table>;

struct Text {
    static Text simple(std::string&& str);
    std::strong_ordering operator<=>(const Text&) const = default;

    std::vector<TextObject> parts;
};

inline Text Text::simple(std::string&& str) {
    return Text{.parts = {Paragraph{.parts = {SimpleText{.str = str, .bold = false, .italic = false}}}}};
}

} // namespace dnd

#endif // TEXT_HPP_
