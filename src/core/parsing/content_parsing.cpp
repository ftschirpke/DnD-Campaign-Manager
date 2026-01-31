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
#include <core/parsing/content_file_parser.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/parsing/spell_file_parser.hpp>
#include <core/parsing/spell_sources_file_parser.hpp>
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

static Opt<const std::filesystem::path> isa_valid_dir(const std::filesystem::path& path) {
    if (std::filesystem::exists(path) && std::filesystem::is_directory(path)) {
        return path;
    }
    return std::nullopt;
}

static Opt<const std::filesystem::path> isa_valid_file(const std::filesystem::path& path) {
    if (std::filesystem::exists(path) && std::filesystem::is_regular_file(path)) {
        return path;
    }
    return std::nullopt;
}

ParsingResult parse_content(const std::set<std::filesystem::path>& content_paths) {
    DND_MEASURE_FUNCTION();
    ParsingResult result;
    result.content_paths = content_paths;

    for (const std::filesystem::path& content_path : content_paths) {
        bool valid_directory = true;
        if (!std::filesystem::exists(content_path)) {
            result.errors.add_parsing_error(
                ParsingError::Code::FILE_NOT_FOUND, content_path, "The content directory does not exist."
            );
            valid_directory = false;
        } else if (!std::filesystem::directory_entry(content_path).is_directory()) {
            result.errors.add_parsing_error(
                ParsingError::Code::FILE_NOT_FOUND, content_path, "The content directory is not a directory."
            );
            valid_directory = false;
        }
        if (!valid_directory) {
            return result;
        }

        if (auto main = isa_valid_file(content_path / "feats.json")) {
            auto foundry = isa_valid_file(content_path / "foundry-feats.json");
            result.errors += parse_file(result.content, ContentFileParser(main.value(), foundry));
        }

        if (auto main = isa_valid_file(content_path / "races.json")) {
            auto foundry = isa_valid_file(content_path / "foundry-races.json");
            result.errors += parse_file(result.content, ContentFileParser(main.value(), foundry));
        }
        if (auto main = isa_valid_file(content_path / "species.json")) {
            auto foundry = isa_valid_file(content_path / "foundry-species.json");
            result.errors += parse_file(result.content, ContentFileParser(main.value(), foundry));
        }

        if (auto dir = isa_valid_dir(content_path / "class")) {
            auto foundry = isa_valid_file(dir.value() / "foundry.json");
            for (const auto& dir_entry : std::filesystem::directory_iterator(dir.value())) {
                if (skip_file(dir_entry.path())) {
                    continue;
                }
                if (auto main = isa_valid_file(dir_entry.path())) {
                    result.errors += parse_file(result.content, ContentFileParser(main.value(), foundry));
                }
            }
        }

        if (auto dir = isa_valid_dir(content_path / "spells")) {
            std::filesystem::path sources_path = dir.value() / "sources.json";
            SpellSourcesFileParser source_parser(sources_path);
            if (isa_valid_file(sources_path)) {
                result.errors += parse_file(result.content, source_parser);
            }

            auto foundry = isa_valid_file(dir.value() / "foundry.json");
            for (const auto& dir_entry : std::filesystem::directory_iterator(dir.value())) {
                if (skip_file(dir_entry.path())) {
                    continue;
                }
                if (auto main = isa_valid_file(dir_entry.path())) {
                    result.errors += parse_file(
                        result.content,
                        SpellFileParser(dir_entry.path(), foundry, source_parser.spell_classes_by_source)
                    );
                }
            }
        }

        if (auto dir = isa_valid_dir(content_path / "characters")) {
            auto foundry = isa_valid_file(content_path / "foundry.json");
            for (const auto& dir_entry : std::filesystem::directory_iterator(content_path / "characters")) {
                if (skip_file(dir_entry.path())) {
                    continue;
                }
                if (auto main = isa_valid_file(dir_entry.path())) {
                    result.errors += parse_file(result.content, ContentFileParser(main.value(), foundry));
                }
            }
        }
    }

    return result;
}


} // namespace dnd
