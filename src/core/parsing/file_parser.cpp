#include <dnd_config.hpp>

#include "file_parser.hpp"

#include <filesystem>
#include <fstream>

#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>

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

} // namespace dnd
