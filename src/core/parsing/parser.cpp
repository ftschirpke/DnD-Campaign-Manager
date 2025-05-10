#include <dnd_config.hpp>

#include "parser.hpp"

#include <algorithm>
#include <deque>
#include <filesystem>
#include <optional>
#include <string>

#include <fmt/format.h>
#include <fmt/ranges.h>
#include <tl/expected.hpp>

#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/text/rich_text.hpp>
#include <core/text/text.hpp>
#include <core/types.hpp>
#include <log.hpp>

namespace dnd {

constexpr std::array<std::string_view, 34> known_link_types = {
    "damage",       "condition",   "dice",   "skill",       "spell", "creature", "action", "adventure",
    "quickref",     "item",        "sense",  "dc",          "note",  "filter",   "chance", "status",
    "classFeature", "variantrule", "hazard", "5etools",     "book",  "feat",     "deity",  "subclassFeature",
    "language",     "class",       "table",  "itemMastery", "deck",  "hit",      "object", "race",
    "optfeature",
};


constexpr std::array<std::string_view, 1> known_ignore_types = {"d20"};

const std::filesystem::path& Parser::get_filepath() const { return filepath; }

Parser::Parser(const std::filesystem::path& filepath) : filepath(filepath) {}

static std::optional<Error> parse_text_recursive(
    std::string&& str, Paragraph& paragraph, bool italic, bool bold, const std::filesystem::path& filepath
) {
    // TODO: add warning if '\n' is left in the string, as this is not well supported (currently)
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
            paragraph.parts.emplace_back(SimpleText{.str = std::string(start, cur), .bold = bold, .italic = italic});
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
                .bold = bold,
                .italic = italic,
            });
        }
        cur += rich_text->length;
        start = cur;
    }
    if (start != cur) {
        paragraph.parts.emplace_back(SimpleText{.str = std::string(start, cur), .bold = bold, .italic = italic});
    }
    return std::nullopt;
}

std::optional<Error> parse_paragraph(std::string&& str, Paragraph& paragraph, const std::filesystem::path& filepath) {
    return parse_text_recursive(std::move(str), paragraph, false, false, filepath);
}

tl::expected<Table, Error> parse_table(const nlohmann::json& json, const std::filesystem::path& filepath) {
    std::optional<Error> error;
    Table new_table{};

    std::string caption;
    error = parse_optional_attribute_into(json, "caption", caption, filepath);
    if (error.has_value()) {
        return tl::unexpected(error.value());
    }
    if (!caption.empty()) {
        new_table.caption = caption;
    }

    error = check_required_attribute(json, "colLabels", filepath, JsonType::ARRAY);
    if (error.has_value()) {
        return tl::unexpected(error.value());
    }
    const nlohmann::json& labels = json["colLabels"];
    new_table.columns = labels.size();

    for (size_t col = 0; col < new_table.columns; ++col) {
        std::string header_entry;
        error = parse_required_index_into(labels, col, header_entry, filepath);
        if (error.has_value()) {
            return tl::unexpected(error.value());
        }
        if (header_entry.starts_with("{@")) {
            // INFO: I think there is no need and no real use case for rich text in the header; reconsider later
            std::optional<RichText> rich_text = parse_rich_text(header_entry);
            if (rich_text.has_value()) {
                header_entry = rich_text->text;
            }
        }
        new_table.header.push_back(header_entry);
    }

    if (json.contains("colStyles") && json["colStyles"].is_array()) {
        const nlohmann::json& styles = json["colStyles"];
        for (size_t col = 0; col < new_table.columns; ++col) {
            if (col >= styles.size()) {
                new_table.column_widths->push_back(std::nullopt);
                continue;
            }
            std::string style_entry;
            error = parse_required_index_into(styles, col, style_entry, filepath);
            if (error.has_value()) {
                return tl::unexpected(error.value());
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
                return tl::unexpected(ParsingError(
                    ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath,
                    "Table column style must start with \"col-<column width>...\""
                ));
            }
        }
    }

    error = check_required_attribute(json, "rows", filepath, JsonType::ARRAY);
    if (error.has_value()) {
        return tl::unexpected(error.value());
    }
    const nlohmann::json& rows = json["rows"];
    for (size_t i = 0; i < rows.size(); ++i) {
        error = check_required_index(rows, i, filepath, JsonType::ARRAY);
        if (error.has_value()) {
            return tl::unexpected(error.value());
        }
        std::vector<Paragraph>& row = new_table.rows.emplace_back();
        for (size_t j = 0; j < rows[i].size(); ++j) {
            std::string entry;
            const nlohmann::json& entry_json = rows[i][j];
            if (entry_json.is_string()) {
                error = parse_required_index_into(rows[i], j, entry, filepath);
                if (error.has_value()) {
                    return tl::unexpected(error.value());
                }
            } else if (entry_json.is_number_integer()) {
                int val;
                error = parse_required_index_into(rows[i], j, val, filepath);
                if (error.has_value()) {
                    return tl::unexpected(error.value());
                }
                entry = fmt::format("{}", val);
            } else {
                error = check_required_index(rows[i], j, filepath, JsonType::OBJECT);
                if (error.has_value()) {
                    return tl::unexpected(error.value());
                }
                std::string typ;
                error = parse_required_attribute_into(entry_json, "type", typ, filepath);
                if (error.has_value()) {
                    return tl::unexpected(error.value());
                }
                if (typ != "cell") {
                    return tl::unexpected(ParsingError(
                        ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath,
                        "When having an object in a table entry, it must be of type \"cell\""
                    ));
                }
                error = check_required_attribute(entry_json, "roll", filepath, JsonType::OBJECT);
                if (error.has_value()) {
                    return tl::unexpected(error.value());
                }
                const nlohmann::json& roll = entry_json["roll"];
                if (roll.contains("exact")) {
                    int exact_roll;
                    error = parse_required_attribute_into(roll, "exact", exact_roll, filepath);
                    if (error.has_value()) {
                        return tl::unexpected(error.value());
                    }
                    entry = fmt::format("{}", exact_roll);
                } else {
                    int min_roll;
                    error = parse_required_attribute_into(roll, "min", min_roll, filepath);
                    if (error.has_value()) {
                        return tl::unexpected(error.value());
                    }
                    int max_roll;
                    error = parse_required_attribute_into(roll, "max", max_roll, filepath);
                    if (error.has_value()) {
                        return tl::unexpected(error.value());
                    }
                    entry = fmt::format("{}-{}", min_roll, max_roll);
                }
            }
            Paragraph entry_paragraph{};
            error = parse_paragraph(std::move(entry), entry_paragraph, filepath);
            if (error.has_value()) {
                return tl::unexpected(error.value());
            }
            row.push_back(entry_paragraph);
        }
    }
    return new_table;
}

static tl::expected<ListItem, Error> parse_list_item(
    const nlohmann::json& json, const std::filesystem::path& filepath
) {
    ListItem out{};

    std::optional<Error> error;
    if (json.is_string()) {
        Paragraph new_paragraph{};
        error = parse_paragraph(json.get<std::string>(), new_paragraph, filepath);
        if (error.has_value()) {
            return tl::unexpected(error.value());
        }
        if (!new_paragraph.parts.empty()) {
            out.parts.push_back(new_paragraph);
        }
        return out;
    }
    if (!json.is_object()) {
        return tl::unexpected(ParsingError(
            ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath,
            "Json entries in the \"items\" array must either be strings or objects."
        ));
    }

    std::string type;
    error = parse_required_attribute_into(json, "type", type, filepath);
    if (error.has_value()) {
        return tl::unexpected(error.value());
    }
    if (type != "item" && type != "itemSpell") {
        return out;
    }

    std::optional<Paragraph> first_paragraph = std::nullopt;
    if (json.contains("name")) {
        std::string name;
        error = parse_required_attribute_into(json, "name", name, filepath);
        if (error.has_value()) {
            return tl::unexpected(error.value());
        }
        if (!name.empty()) {
            if (!first_paragraph.has_value()) {
                first_paragraph = Paragraph{};
            }
            first_paragraph->parts.push_back(SimpleText{
                .str = name + ". ",
                .bold = true,
                .italic = false,
            });
        }
    }

    std::deque<CRef<nlohmann::json>> todo;

    if (json.contains("entry")) {
        todo.push_back(json["entry"]);
    } else {
        error = check_required_attribute(json, "entries", filepath, JsonType::ARRAY);
        if (error.has_value()) {
            return tl::unexpected(error.value());
        }
        for (const nlohmann::json& entry : json["entries"]) {
            todo.push_back(entry);
        }
    }

    if (first_paragraph.has_value()) {
        const nlohmann::json& first_entry = todo.front();
        if (first_entry.is_string()) {
            todo.pop_front();
            error = parse_paragraph(first_entry.get<std::string>(), first_paragraph.value(), filepath);
            if (error.has_value()) {
                return tl::unexpected(error.value());
            }
        }
        if (!first_paragraph.value().parts.empty()) {
            out.parts.push_back(first_paragraph.value());
        }
    }

    while (!todo.empty()) {
        const nlohmann::json& entry = todo.front();
        todo.pop_front();
        if (entry.is_string()) {
            Paragraph new_paragraph{};
            error = parse_paragraph(entry.get<std::string>(), new_paragraph, filepath);
            if (error.has_value()) {
                return tl::unexpected(error.value());
            }
            if (!new_paragraph.parts.empty()) {
                out.parts.push_back(new_paragraph);
            }
            continue;
        }

        if (!entry.is_object()) {
            return tl::unexpected(ParsingError(
                ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath,
                "Json entries in the \"entries\" array must either be strings or objects."
            ));
        }

        std::string entry_type;
        error = parse_required_attribute_into(entry, "type", entry_type, filepath);
        if (error.has_value()) {
            return tl::unexpected(error.value());
        }

        if (entry_type == "entries") {
            error = check_required_attribute(entry, "entries", filepath, JsonType::ARRAY);
            if (error.has_value()) {
                return tl::unexpected(error.value());
            }

            if (entry.contains("name")) {
                std::string name;
                error = parse_required_attribute_into(entry, "name", name, filepath);
                if (error.has_value()) {
                    return tl::unexpected(error.value());
                }
                Paragraph new_paragraph{};
                if (!name.empty()) {
                    new_paragraph.parts.push_back(SimpleText{.str = name + ". ", .bold = true, .italic = true});
                }

                std::vector<std::string> entries_strings;
                error = parse_required_attribute_into(entry, "entries", entries_strings, filepath);
                if (error.has_value()) {
                    // fallback, treat all entries on their own
                    for (auto it = entry["entries"].rbegin(); it != entry["entries"].rend(); ++it) {
                        todo.push_front(*it);
                    }
                    error = std::nullopt;
                } else {
                    error = parse_paragraph(
                        fmt::format("{}", fmt::join(entries_strings.begin(), entries_strings.end(), " ")),
                        new_paragraph, filepath
                    );
                    if (error.has_value()) {
                        return tl::unexpected(error.value());
                    }
                }
                if (!new_paragraph.parts.empty()) {
                    out.parts.push_back(new_paragraph);
                }
            } else {
                for (auto it = entry["entries"].rbegin(); it != entry["entries"].rend(); ++it) {
                    todo.push_front(*it);
                }
            }
        } else if (entry_type == "table") {
            tl::expected<Table, Error> new_table = parse_table(entry, filepath);
            if (new_table.has_value()) {
                out.parts.push_back(new_table.value());
            } else {
                return tl::unexpected(new_table.error());
            }
        } else {
            return tl::unexpected(ParsingError(
                ParsingError::Code::UNEXPECTED_ATTRIBUTE, filepath,
                fmt::format("Entry type \"{}\" unexpected", entry_type)
            ));
        }
    }
    return out;
}

std::optional<Error> parse_list(const nlohmann::json& list_items, Text& out, const std::filesystem::path& filepath) {
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
        tl::expected<ListItem, Error> parsed_item = parse_list_item(item, filepath);
        if (parsed_item.has_value()) {
            if (!parsed_item.value().parts.empty()) {
                new_list.parts.push_back(parsed_item.value());
            }
        } else {
            return parsed_item.error();
        }
    }

    if (new_list.parts.size() == 1) {
        for (std::variant<Paragraph, Table>& part : new_list.parts[0].parts) {
            switch (part.index()) {
                case 0: /*Paragraph */ {
                    Paragraph& paragraph = std::get<0>(part);
                    if (!paragraph.parts.empty()) {
                        out.parts.push_back(paragraph);
                    }
                    break;
                }
                case 1: /* Table */ {
                    out.parts.push_back(std::get<1>(part));
                    break;
                }
                default:
                    assert(false);
            }
        }
    } else if (!new_list.parts.empty()) {
        out.parts.push_back(new_list);
    }
    return std::nullopt;
}

std::optional<Error> write_formatted_text_into(
    const nlohmann::json& json, Text& out, const std::filesystem::path& filepath
) {
    return write_formatted_text_into(json, out, filepath, false);
}

std::optional<Error> write_formatted_text_into(
    const nlohmann::json& json, Text& out, const std::filesystem::path& filepath, bool skip_first
) {
    std::optional<Error> error = check_required_attribute(json, "entries", filepath, JsonType::ARRAY);
    if (error.has_value()) {
        return error;
    }

    std::deque<CRef<nlohmann::json>> todo;

    bool is_first = true;
    for (const nlohmann::json& entry : json["entries"]) {
        if (skip_first && is_first) {
            is_first = false;
            continue;
        }
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
                Paragraph new_paragraph{};
                if (!name.empty()) {
                    new_paragraph.parts.push_back(SimpleText{.str = name + ". ", .bold = true, .italic = true});
                }

                std::vector<std::string> entries_strings;
                error = parse_required_attribute_into(entry, "entries", entries_strings, filepath);
                if (error.has_value()) {
                    // fallback, treat all entries on their own
                    for (auto it = entry["entries"].rbegin(); it != entry["entries"].rend(); ++it) {
                        todo.push_front(*it);
                    }
                    error = std::nullopt;
                } else {
                    error = parse_paragraph(
                        fmt::format("{}", fmt::join(entries_strings.begin(), entries_strings.end(), " ")),
                        new_paragraph, filepath
                    );
                    if (error.has_value()) {
                        return error;
                    }
                }
                if (!new_paragraph.parts.empty()) {
                    out.parts.push_back(new_paragraph);
                }
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
            tl::expected<Table, Error> new_table = parse_table(entry, filepath);
            if (new_table.has_value()) {
                out.parts.push_back(new_table.value());
            } else {
                return new_table.error();
            }
        } else if (type == "options" || type.starts_with("ref") || type == "abilityDc" || type == "abilityAttackMod"
                   || type == "statblock" || type == "quote" // TODO: implement, might be nice
        ) {
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
}

static std::string merge_strings(
    const std::vector<std::string>& strs, const char* connector, const char* last_connector
) {
    if (strs.empty()) {
        return "";
    }
    if (strs.size() == 1) {
        return strs[0];
    }

    return fmt::format("{} {} {}", fmt::join(strs.cbegin(), strs.cend() - 1, connector), last_connector, strs.back());
}

std::string or_of_strings(const std::vector<std::string>& strs) { return merge_strings(strs, ", ", "or"); }

std::string and_of_strings(const std::vector<std::string>& strs) { return merge_strings(strs, ", ", "and"); }

} // namespace dnd
