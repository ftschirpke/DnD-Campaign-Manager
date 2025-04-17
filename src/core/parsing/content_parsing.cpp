#include <dnd_config.hpp>

#include "content_parsing.hpp"

#include <exception>
#include <filesystem>
#include <string>

#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/errors/validation_error.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/parsing/spell_file_parser.hpp>
#include <core/parsing/spell_sources_file_parser.hpp>
#include <core/parsing/v2_file_parser.hpp>
#include <log.hpp>

namespace dnd {

static Errors parse_file(Content& content, FileParser& parser) {
    DND_MEASURE_SCOPE(parser.get_filepath().string().c_str());
    Errors errors;
    try {
        errors += parser.open_json();
        if (!errors.ok()) {
            return errors;
        }

        errors += parser.parse();
        if (!errors.ok() && !parser.continue_after_errors()) {
            return errors;
        }
    } catch (const std::exception& e) {
        errors.add_parsing_error(ParsingError::Code::UNKNOWN_ERROR, parser.get_filepath(), e.what());
        return errors;
    }
    parser.set_context(content);
    parser.save_result(content);
    return errors;
}

static Errors parse_file(Content& content, FileParser&& parser) { return parse_file(content, parser); }

static bool skip_file(const std::filesystem::path& filepath) {
    std::string filename = filepath.filename().string();
    if (filename.size() >= 7 && filename.substr(0, 7) == "foundry") {
        return true;
    }
    if (filename.size() >= 10 && filename.substr(filename.size() - 10) == "index.json") {
        return true;
    }
    if (filename == "class-sidekick.json") {
        return true;
    }
    return false;
}

ParsingResult parse_content(const std::filesystem::path& content_path, const std::string& campaign_dir_name) {
    DND_MEASURE_FUNCTION();
    ParsingResult result;
    result.content_path = content_path;
    result.campaign_directory_name = campaign_dir_name;

    if (!std::filesystem::exists(content_path)) {
        result.errors.add_parsing_error(
            ParsingError::Code::FILE_NOT_FOUND, content_path, "The content directory does not exist."
        );
    } else if (!std::filesystem::directory_entry(content_path).is_directory()) {
        result.errors.add_parsing_error(
            ParsingError::Code::FILE_NOT_FOUND, content_path, "The content directory is not a directory."
        );
    }
    if (!result.errors.ok()) {
        return result;
    }

    if (std::filesystem::exists(content_path / "class") && std::filesystem::is_directory(content_path / "class")) {
        for (const auto& dir_entry : std::filesystem::directory_iterator(content_path / "class")) {
            if (std::filesystem::is_directory(dir_entry) || skip_file(dir_entry.path())) {
                continue;
            }
            result.errors += parse_file(result.content, V2FileParser(dir_entry.path()));
        }
    }

    if (std::filesystem::exists(content_path / "races.json")
        && std::filesystem::is_regular_file(content_path / "races.json")) {
        result.errors += parse_file(result.content, V2FileParser(content_path / "races.json"));
    }

    if (std::filesystem::exists(content_path / "spells") && std::filesystem::is_directory(content_path / "spells")) {
        std::filesystem::path sources_path = content_path / "spells" / "sources.json";
        SpellSourcesFileParser source_parser(sources_path);
        result.errors += parse_file(result.content, source_parser);

        for (const auto& dir_entry : std::filesystem::directory_iterator(content_path / "spells")) {
            if (std::filesystem::is_directory(dir_entry) || skip_file(dir_entry.path())) {
                continue;
            }
            result.errors += parse_file(
                result.content, SpellFileParser(dir_entry.path(), source_parser.spell_classes_by_source)
            );
        }
    }

    return result;
}


} // namespace dnd
