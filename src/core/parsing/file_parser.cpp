#include <dnd_config.hpp>

#include "file_parser.hpp"

#include <filesystem>
#include <fstream>

#include <fmt/format.h>
#include <fmt/ranges.h>
#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <log.hpp>

namespace dnd {

FileParser::FileParser(const std::filesystem::path& filepath, bool multiple_pieces_per_file)
    : Parser(filepath), multiple_pieces_per_file(multiple_pieces_per_file) {}

Errors FileParser::open_json() {
    DND_MEASURE_FUNCTION();
    Errors errors;
    if (get_filepath().extension().string() != ".json") {
        errors.add_parsing_error(
            ParsingError::Code::INVALID_FILE_FORMAT, get_filepath(),
            fmt::format("File '{}' is not a \".json\" file.", get_filepath().string())
        );
        return errors;
    }
    std::ifstream json_file(get_filepath());
    try {
        DND_MEASURE_SCOPE("JSON serialization");
        json_file >> json;
    } catch (const nlohmann::json::parse_error& e) {
        errors.add_parsing_error(
            ParsingError::Code::INVALID_FILE_FORMAT, get_filepath(),
            fmt::format("Error occured while parsing '{}': {} ", get_filepath().string(), e.what())
        );
    }
    return errors;
}

bool FileParser::continue_after_errors() const { return multiple_pieces_per_file; }

void FileParser::set_context(const Content& content) { DND_UNUSED(content); }

FoundryFileParser::FoundryFileParser(Opt<CRef<std::filesystem::path>> filepath) : filepath(filepath) {}

Errors FoundryFileParser::open_json() {
    DND_MEASURE_FUNCTION();
    Errors errors;
    if (!filepath.has_value()) {
        return errors;
    }
    const std::filesystem::path& path = filepath.value();
    if (path.extension().string() != ".json") {
        errors.add_parsing_error(
            ParsingError::Code::INVALID_FILE_FORMAT, path,
            fmt::format("File '{}' is not a \".json\" file.", path.string())
        );
        return errors;
    }
    std::ifstream json_file(path);
    try {
        DND_MEASURE_SCOPE("JSON serialization");
        json_file >> json;
    } catch (const nlohmann::json::parse_error& e) {
        errors.add_parsing_error(
            ParsingError::Code::INVALID_FILE_FORMAT, path,
            fmt::format("Error occured while parsing '{}': {} ", path.string(), e.what())
        );
    }
    return errors;
}

static Opt<CRef<nlohmann::json>> get_entry(const nlohmann::json& obj, std::string type_key, const std::string& key) {
    DND_MEASURE_FUNCTION();
    if (type_key == "Choosable") {
        // HACK: currently only parsing feats, thus we can just overwrite it; but better solution required
        type_key = "feat";
    } else {
        type_key[0] = std::tolower(type_key[0]);
    }

    if (!obj.contains(type_key)) {
        return std::nullopt;
    }

    std::vector<std::string> key_parts;

    std::string::const_iterator start = key.cbegin();
    std::string::const_iterator end = start;

    bool in_part = true;
    while (end != key.cend()) {
        if (*end == '#' || *end == '|') {
            if (in_part) {
                key_parts.emplace_back(start, end);
                in_part = false;
            }
        } else {
            if (!in_part) {
                start = end;
                in_part = true;
            }
        }
        ++end;
    }
    key_parts.emplace_back(start, end);

    std::vector<CRef<nlohmann::json>> matches;
    for (const nlohmann::json& entry : obj[type_key]) {
        if (!entry.contains("name")) {
            LOGWARN("Missing 'name' key in entry for get_entry function while searching '{}:{}'", type_key, key);
            continue;
        }
        if (entry["name"] == key_parts[0]) {
            matches.emplace_back(entry);
        }
    }

    constexpr std::array<std::string_view, 1> secondary_criteria = {"source"};
    size_t idx = 1;
    while (idx < key_parts.size() && idx - 1 < secondary_criteria.size()) {
        auto criterium = secondary_criteria[idx - 1];
        for (auto it = matches.begin(); it != matches.end();) {
            const nlohmann::json& entry = *it;
            if (!entry.contains(criterium)) {
                LOGWARN(
                    "Missing '{}' key in entry for get_entry function while searching '{}:{}'", criterium, type_key, key
                );
                continue;
            }
            if (entry[criterium] == key_parts[idx]) {
                ++it;
            } else {
                it = matches.erase(it);
            }
        }
        ++idx;
    }

    if (matches.size() == 1) {
        return matches[0];
    } else if (matches.size() > 1) {
        LOGWARN(
            "Could not uniquely identify foundry entry for '{}:{}' search - found {}", type_key, key, matches.size()
        );
        return matches[0];
    }

    return std::nullopt;
}

#define IMPL_GET_OBJ_KEY(C, U, j, a, p, P)                                                                             \
    Opt<CRef<nlohmann::json>> FoundryFileParser::get_##j##_entry(const std::string& key) const {                       \
        if (!filepath.has_value()) {                                                                                   \
            return std::nullopt;                                                                                       \
        }                                                                                                              \
        return get_entry(json, #C, key);                                                                               \
    }
X_CONTENT_PIECES(IMPL_GET_OBJ_KEY)
#undef IMPL_GET_OBJ_KEY

} // namespace dnd
