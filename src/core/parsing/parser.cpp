#include <dnd_config.hpp>

#include "parser.hpp"

#include <algorithm>
#include <deque>
#include <filesystem>
#include <optional>
#include <string>

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/text/rich_text.hpp>
#include <core/text/text.hpp>
#include <core/types.hpp>
#include <log.hpp>

namespace dnd {

constexpr std::array<std::string_view, 29> known_link_types = {
    "damage",       "condition",   "dice",   "skill",       "spell", "creature", "action", "adventure",
    "quickref",     "item",        "sense",  "dc",          "note",  "filter",   "chance", "status",
    "classFeature", "variantrule", "hazard", "5etools",     "book",  "feat",     "deity",  "subclassFeature",
    "language",     "class",       "table",  "itemMastery", "deck"
};

constexpr std::array<std::string_view, 1> known_ignore_types = {"d20"};

const std::filesystem::path& Parser::get_filepath() const { return filepath; }

Parser::Parser(const std::filesystem::path& filepath) : filepath(filepath) {}

static std::optional<Error> parse_text_recursive(
    std::string&& str, Paragraph& paragraph, bool italic, bool bold, const std::filesystem::path& filepath
) {
    std::optional<Error> error;
    std::string::iterator start = str.begin();
    std::string::iterator cur = str.begin();
    std::string::iterator end = str.end();
    while (cur != end) {
        if (*cur != '{') {
            ++cur;
            continue;
        }
        std::optional<RichText> rich_text = parse_rich_text(cur, end);
        if (!rich_text.has_value()) {
            ++cur;
            continue;
        }
        if (start != cur) {
            paragraph.parts.emplace_back(SimpleText{.str = std::string(start, cur), .italic = italic, .bold = bold});
        }

        if (rich_text->rich_type == "i" || rich_text->rich_type == "b") {
            if (!rich_text->attributes.empty()) {
                return ParsingError(
                    ParsingError::Code::UNEXPECTED_ATTRIBUTE, filepath,
                    "Rich text that is just a modifier (i.e. \"i\" for italic or \"b\" bold cannot have attributes as "
                    "well"
                );
            }
            if (rich_text->rich_type == "i") {
                italic = true;
            } else {
                assert(rich_text->rich_type == "b");
                bold = true;
            }
            error = parse_text_recursive(std::move(rich_text->text), paragraph, italic, bold, filepath);
            if (error.has_value()) {
                return error;
            }
        } else if (std::find(known_ignore_types.begin(), known_ignore_types.end(), rich_text->rich_type)
                   != known_ignore_types.end()) {
        } else {
            if (std::find(known_link_types.begin(), known_link_types.end(), rich_text->rich_type)
                == known_link_types.end()) {
                LOGWARN("Found rich text of unknown type '{}' - assuming link", rich_text->rich_type);
            }
            paragraph.parts.emplace_back(Link{
                .str = std::move(rich_text->text),
                .attributes = std::move(rich_text->attributes),
                .italic = italic,
                .bold = bold,
            });
        }
        cur += rich_text->length;
        start = cur;
    }
    if (start != cur) {
        paragraph.parts.emplace_back(SimpleText{.str = std::string(start, cur), .italic = italic, .bold = bold});
    }
    return std::nullopt;
}

static std::optional<Error> parse_paragraph(
    std::string&& str, Paragraph& paragraph, const std::filesystem::path& filepath
) {
    return parse_text_recursive(std::move(str), paragraph, false, false, filepath);
}

static std::optional<Error> parse_list(
    const nlohmann::json& list_items, Text& out, const std::filesystem::path& filepath
) {
    std::optional<Error> error;
    List new_list{};

    if (!out.parts.empty()) {
        TextObject& last = out.parts.back();
        if (last.index() == 0) /* Paragraph */ {
            Paragraph& paragraph = std::get<0>(last);
            if (!paragraph.parts.empty()) {
                const InlineText& last_inline = paragraph.parts.back();
                if (last_inline.index() == 0) /* SimpleText */ {
                    const SimpleText& simple_text = std::get<0>(last_inline);
                    if (simple_text.str.ends_with(':')) {
                        new_list.text_above = paragraph;
                        out.parts.pop_back();
                    }
                }
            }
        }
    }

    for (const nlohmann::json& item : list_items) {
        Paragraph new_paragraph{};
        if (item.is_string()) {
            error = parse_paragraph(item.get<std::string>(), new_paragraph, filepath);
            if (error.has_value()) {
                return error;
            }
            new_list.parts.push_back(new_paragraph);
            continue;
        }

        if (!item.is_object()) {
            return ParsingError(
                ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath,
                "Json entries in the \"items\" array must either be strings or objects."
            );
        }
        std::string type;
        error = parse_required_attribute_into(item, "type", type, filepath);
        if (error.has_value()) {
            return error;
        }
        if (type != "item") {
            return ParsingError(
                ParsingError::Code::UNEXPECTED_ATTRIBUTE, filepath,
                fmt::format("Item in has unexpected type \"{}\" != \"item\"", type)
            );
        }
        std::vector<std::string> items_strings;
        error = parse_required_attribute_into(item, "entries", items_strings, filepath);
        if (error.has_value()) {
            return error;
        }

        std::string name;
        error = parse_optional_attribute_into(item, "name", name, filepath);
        if (error.has_value()) {
            return error;
        }
        new_paragraph.parts.push_back(SimpleText{.str = name + ". ", .italic = true, .bold = true});

        error = parse_paragraph(
            fmt::format("{}", fmt::join(items_strings.begin(), items_strings.end(), " ")), new_paragraph, filepath
        );
        if (error.has_value()) {
            return error;
        }

        new_list.parts.push_back(new_paragraph);
    }
    out.parts.push_back(new_list);
    return std::nullopt;
}

static std::optional<Error> parse_table(const nlohmann::json& json, Text& out, const std::filesystem::path& filepath) {
    std::optional<Error> error;
    Table new_table{};

    std::string caption;
    error = parse_optional_attribute_into(json, "caption", caption, filepath);
    if (error.has_value()) {
        return error;
    }
    if (!caption.empty()) {
        new_table.caption = caption;
    }

    error = check_required_attribute(json, "colLabels", filepath, JsonType::ARRAY);
    if (error.has_value()) {
        return error;
    }
    const nlohmann::json& labels = json["colLabels"];
    new_table.columns = labels.size();

    for (size_t i = 0; i < new_table.columns; ++i) {
        std::string header_entry;
        error = parse_required_index_into(labels, i, header_entry, filepath);
        if (error.has_value()) {
            return error;
        }
        new_table.header.push_back(header_entry);
    }

    if (json.contains("colStyles") && json["colStyles"].is_array()) {
        const nlohmann::json& styles = json["colStyles"];
        for (size_t i = 0; i < new_table.columns; ++i) {
            if (i >= styles.size()) {
                new_table.column_widths->push_back(std::nullopt);
                continue;
            }
            std::string style_entry;
            error = parse_required_index_into(styles, i, style_entry, filepath);
            if (error.has_value()) {
                return error;
            }
            if (style_entry.starts_with("col-")) {
                size_t i = 4;
                while (i < style_entry.size() && style_entry[i] != ' ') {
                    ++i;
                }
                if (!new_table.column_widths.has_value()) {
                    new_table.column_widths = std::vector<std::optional<int>>();
                }
                new_table.column_widths->push_back(std::stoi(style_entry.substr(4, i)));
            } else {
                return ParsingError(
                    ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath,
                    "Table column style must start with \"col-<column width>...\""
                );
            }
        }
    }

    error = check_required_attribute(json, "rows", filepath, JsonType::ARRAY);
    if (error.has_value()) {
        return error;
    }
    const nlohmann::json& rows = json["rows"];
    for (size_t i = 0; i < new_table.columns; ++i) {
        error = check_required_index(rows, i, filepath, JsonType::ARRAY);
        if (error.has_value()) {
            return error;
        }
        std::vector<Paragraph>& row = new_table.rows.emplace_back();
        for (size_t j = 0; j < rows[i].size(); ++j) {
            std::string entry;
            error = parse_required_index_into(rows[i], j, entry, filepath);
            if (error.has_value()) {
                return error;
            }
            Paragraph entry_paragraph{};
            error = parse_paragraph(std::move(entry), entry_paragraph, filepath);
            if (error.has_value()) {
                return error;
            }
            row.push_back(entry_paragraph);
        }
    }
    out.parts.push_back(new_table);
    return std::nullopt;
}

std::optional<Error> write_formatted_text_into(
    const nlohmann::json& json, Text& out, const std::filesystem::path& filepath
) {
    std::optional<Error> error = check_required_attribute(json, "entries", filepath, JsonType::ARRAY);
    if (error.has_value()) {
        return error;
    }

    std::deque<CRef<nlohmann::json>> todo;

    for (const nlohmann::json& entry : json["entries"]) {
        todo.push_back(entry);
    }

    while (!todo.empty()) {
        const nlohmann::json& entry = todo.front();
        todo.pop_front();
        if (entry.is_string()) {
            Paragraph new_paragraph{};
            error = parse_paragraph(entry.get<std::string>(), new_paragraph, filepath);
            if (error.has_value()) {
                return error;
            }
            if (!new_paragraph.parts.empty()) {
                out.parts.push_back(new_paragraph);
            }
            continue;
        }

        if (!entry.is_object()) {
            return ParsingError(
                ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath,
                "Json entries in the \"entries\" array must either be strings or objects."
            );
        }

        std::string type;
        error = parse_required_attribute_into(entry, "type", type, filepath);
        if (error.has_value()) {
            return error;
        }

        if (type == "entries" || type == "inset" || type == "section") {
            error = check_required_attribute(entry, "entries", filepath, JsonType::ARRAY);
            if (error.has_value()) {
                return error;
            }

            if (entry.contains("name")) {
                std::string name;
                error = parse_required_attribute_into(entry, "name", name, filepath);
                if (error.has_value()) {
                    return error;
                }
                std::vector<std::string> entries_strings;
                error = parse_required_attribute_into(entry, "entries", entries_strings, filepath);
                if (error.has_value()) {
                    return error;
                }
                Paragraph new_paragraph{};
                new_paragraph.parts.push_back(SimpleText{.str = name + ". ", .italic = true, .bold = true});
                error = parse_paragraph(
                    fmt::format("{}", fmt::join(entries_strings.begin(), entries_strings.end(), " ")), new_paragraph,
                    filepath
                );
                if (error.has_value()) {
                    return error;
                }
                out.parts.push_back(new_paragraph);
            } else {
                for (auto it = entry["entries"].rbegin(); it != entry["entries"].rend(); ++it) {
                    todo.push_front(*it);
                }
            }
        } else if (type == "list") {
            error = check_required_attribute(entry, "items", filepath, JsonType::ARRAY);
            if (error.has_value()) {
                return error;
            }
            error = parse_list(entry["items"], out, filepath);
            if (error.has_value()) {
                return error;
            }
        } else if (type == "table") {
            parse_table(entry, out, filepath);
        } else if (type == "options" || (type.starts_with("ref") && type.ends_with("Feature"))) {
            continue;
        } else {
            return ParsingError(
                ParsingError::Code::UNEXPECTED_ATTRIBUTE, filepath, fmt::format("Entry type \"{}\" unexpected", type)
            );
        }
    }
    return std::nullopt;
}

static const char* json_attribute_type_name(JsonType typ) {
    switch (typ) {
        case JsonType::STRING:
            return "string";
        case JsonType::ARRAY:
            return "array";
        case JsonType::OBJECT:
            return "object";
        case JsonType::ANY:
            return "any";
        default:
            assert(false);
            return "";
    }
}

static std::optional<Error> check_type(
    const nlohmann::json& json, JsonType typ, const std::filesystem::path& filepath, std::string&& error_msg
) {
    bool is_required_type = false;
    switch (typ) {
        case JsonType::STRING:
            is_required_type = json.is_string();
            break;
        case JsonType::ARRAY:
            is_required_type = json.is_array();
            break;
        case JsonType::OBJECT:
            is_required_type = json.is_object();
            break;
        case JsonType::ANY:
            is_required_type = true;
            break;
        default:
            assert(false);
            break;
    }
    if (!is_required_type) {
        return ParsingError(ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath, std::move(error_msg));
    }
    return std::nullopt;
}

std::optional<Error> check_required_attribute(
    const nlohmann::json& json, const char* attribute_name, const std::filesystem::path& filepath, JsonType typ
) {
    if (!json.contains(attribute_name)) {
        return ParsingError(
            ParsingError::Code::MISSING_ATTRIBUTE, filepath,
            fmt::format("The attribute '{}' is missing", attribute_name)
        );
    }
    return check_type(
        json[attribute_name], typ, filepath,
        fmt::format(
            "The attribute '{}' exists but is of wrong type - should be {}", attribute_name,
            json_attribute_type_name(typ)
        )
    );
}

std::optional<Error> check_required_index(
    const nlohmann::json& json, size_t index, const std::filesystem::path& filepath, JsonType typ
) {
    if (json.size() <= index) {
        return ParsingError(
            ParsingError::Code::MISSING_ATTRIBUTE, filepath, fmt::format("Array does not have index {}", index)
        );
    }
    return check_type(
        json[index], typ, filepath,
        fmt::format("The index {} exists but is of wrong type - should be {}", index, json_attribute_type_name(typ))
    );
    return std::nullopt;
}

} // namespace dnd
