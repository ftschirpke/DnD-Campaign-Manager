#include <dnd_config.hpp>

#include "file_parser.hpp"

#include <filesystem>
#include <fstream>

#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>

dnd::FileParser::FileParser(const std::filesystem::path& filepath) noexcept : Parser(filepath) {}

dnd::Errors dnd::FileParser::open_json() {
    DND_MEASURE_FUNCTION();
    Errors errors;
    if (get_filepath().extension().string() != ".json") {
        errors.add_parsing_error(
            ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(),
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
            ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(),
            fmt::format("Error occured while parsing '{}': {} ", get_filepath().string(), e.what())
        );
    }
    return errors;
}

bool dnd::FileParser::continue_after_invalid_parsing() const noexcept { return false; }

void dnd::FileParser::set_context(const dnd::ContentHolder& content) { DND_UNUSED(content); }
