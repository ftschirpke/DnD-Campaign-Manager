#ifndef TEXT_OBJECT_HPP_
#define TEXT_OBJECT_HPP_

#include <dnd_config.hpp>

#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace dnd {

struct SimpleText {
    std::string str;
    bool italic = false;
    bool bold = false;
};

struct Link {
    std::string str;
    std::string typ;
    std::string search_key;
};

using InlineText = std::variant<SimpleText, Link>;

struct Paragraph {
    std::vector<InlineText> parts;
};

struct List {
    std::vector<InlineText> parts;
    bool enumerated = false;
    std::optional<Paragraph> text_before;
    std::optional<Paragraph> text_after;
};

struct Table {
    using Row = std::vector<std::string>;
    std::optional<Row> header;
    std::vector<Row> rows;
};

using Text = std::variant<Paragraph, List, Table>;

} // namespace dnd

#endif // TEXT_OBJECT_HPP_
