#include <dnd_config.hpp>

#include "content_parsing.hpp"

#include <filesystem>
#include <functional>
#include <future>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/errors/validation_error.hpp>
#include <core/parsing/character/character_parser.hpp>
#include <core/parsing/character_class/character_class_parser.hpp>
#include <core/parsing/character_race/character_race_parser.hpp>
#include <core/parsing/character_subclass/character_subclass_parser.hpp>
#include <core/parsing/character_subrace/character_subrace_parser.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/parsing/groups/choosable_group_parser.hpp>
#include <core/parsing/groups/string_group_parser.hpp>
#include <core/parsing/item/item_parser.hpp>
#include <core/parsing/spell/spell_parser.hpp>

namespace {

enum class ParsingType {
    CHARACTER,
    RACE,
    SUBRACE,
    CLASS,
    SUBCLASS,
    ITEM,
    SPELL,
    CHOOSABLES,
};

} // namespace


static dnd::Errors parse_all_of_type(
    ParsingType type, dnd::Content& content, const std::vector<std::filesystem::directory_entry>& content_directories
);
static dnd::Errors parse_string_groups(
    dnd::Content& content, const std::vector<std::filesystem::directory_entry>& content_directories
);


dnd::ParsingResult dnd::parse_content(const std::filesystem::path& content_path, const std::string& campaign_dir_name) {
    DND_MEASURE_FUNCTION();
    ParsingResult result;
    result.content_path = content_path;
    result.campaign_directory_name = campaign_dir_name;

    if (!std::filesystem::exists(content_path)) {
        result.errors.add_parsing_error(
            ParsingErrorCode::FILE_NOT_FOUND, content_path, "The content directory does not exist."
        );
    } else if (!std::filesystem::directory_entry(content_path).is_directory()) {
        result.errors.add_parsing_error(
            ParsingErrorCode::FILE_NOT_FOUND, content_path, "The content directory is not a directory."
        );
    } else {
        if (!std::filesystem::exists(content_path / "general")) {
            result.errors.add_parsing_error(
                ParsingErrorCode::FILE_NOT_FOUND, content_path / "general", "The general directory does not exist."
            );
        }
        if (!std::filesystem::exists(content_path / campaign_dir_name)) {
            result.errors.add_parsing_error(
                ParsingErrorCode::FILE_NOT_FOUND, content_path / campaign_dir_name,
                "The campaign directory does not exist."
            );
        }
    }
    if (!result.errors.ok()) {
        return result;
    }

    std::vector<std::filesystem::directory_entry> content_directories;
    for (const auto& entry : std::filesystem::directory_iterator(content_path / "general")) {
        if (entry.is_directory()) {
            content_directories.emplace_back(std::move(entry));
        }
    }
    content_directories.emplace_back(content_path / campaign_dir_name);

    std::reference_wrapper<Content> content_ref = std::ref(result.content);
    auto content_directories_ref = std::ref(content_directories);
    {
        DND_MEASURE_SCOPE("Parsing: string groups, spells");
        std::future<Errors> string_group_future = std::async(
            std::launch::async, parse_string_groups, content_ref, content_directories_ref
        );
        std::future<Errors> spell_future = std::async(
            std::launch::async, parse_all_of_type, ParsingType::SPELL, content_ref, content_directories_ref
        );
        result.errors += string_group_future.get();
        result.errors += spell_future.get();
    }
    {
        DND_MEASURE_SCOPE("Parsing: items, choosable features, races, classes");
        std::future<Errors> item_future = std::async(
            std::launch::async, parse_all_of_type, ParsingType::ITEM, content_ref, content_directories_ref
        );
        std::future<Errors> choosable_future = std::async(
            std::launch::async, parse_all_of_type, ParsingType::CHOOSABLES, content_ref, content_directories_ref
        );
        std::future<Errors> race_future = std::async(
            std::launch::async, parse_all_of_type, ParsingType::RACE, content_ref, content_directories_ref
        );
        std::future<Errors> class_future = std::async(
            std::launch::async, parse_all_of_type, ParsingType::CLASS, content_ref, content_directories_ref
        );
        result.errors += item_future.get();
        result.errors += choosable_future.get();
        result.errors += race_future.get();
        result.errors += class_future.get();
    }
    {
        DND_MEASURE_SCOPE("Parsing: subraces, subclasses");
        std::future<Errors> subrace_future = std::async(
            std::launch::async, parse_all_of_type, ParsingType::SUBRACE, content_ref, content_directories_ref
        );
        std::future<Errors> subclass_future = std::async(
            std::launch::async, parse_all_of_type, ParsingType::SUBCLASS, content_ref, content_directories_ref
        );
        result.errors += subrace_future.get();
        result.errors += subclass_future.get();
    }
    {
        DND_MEASURE_SCOPE("Parsing characters");
        result.errors += parse_all_of_type(ParsingType::CHARACTER, result.content, content_directories);
    }

    return result;
}

static dnd::Errors parse_file(dnd::Content& content, dnd::FileParser&& parser) {
    DND_MEASURE_SCOPE(parser.get_filepath().string().c_str());
    dnd::Errors errors;
    bool successful = true;
    try {
        dnd::Errors open_json_errors = parser.open_json();
        successful = open_json_errors.ok();
        errors += std::move(open_json_errors);
        if (!successful) {
            return errors;
        }

        dnd::Errors parse_errors = parser.parse();
        successful = parse_errors.ok();
        errors += std::move(parse_errors);
        if (!successful && !parser.continue_after_errors()) {
            return errors;
        }
    } catch (const std::exception& e) {
        errors.add_parsing_error(dnd::ParsingErrorCode::UNKNOWN_ERROR, parser.get_filepath(), e.what());
        return errors;
    }

    try {
        parser.set_context(content);

        dnd::Errors validation_errors = parser.validate(content);
        successful = validation_errors.ok();
        errors += std::move(validation_errors);
        if (!successful && !parser.continue_after_errors()) {
            return errors;
        }

        parser.save_result(content);
    } catch (const std::exception& e) {
        errors.add_validation_error(dnd::ValidationErrorCode::UNKNOWN_ERROR, nullptr, e.what());
    }
    return errors;
}

static dnd::Errors parse_file_of_type(const std::filesystem::path& file, dnd::Content& content, ParsingType type) {
    switch (type) {
        case ParsingType::CHARACTER:
            return parse_file(content, dnd::CharacterParser(file));
        case ParsingType::RACE:
            return parse_file(content, dnd::CharacterRaceParser(file));
        case ParsingType::CLASS:
            return parse_file(content, dnd::CharacterClassParser(file));
        case ParsingType::SUBRACE:
            return parse_file(content, dnd::CharacterSubraceParser(file));
        case ParsingType::SUBCLASS:
            return parse_file(content, dnd::CharacterSubclassParser(file));
        case ParsingType::ITEM:
            return parse_file(content, dnd::ItemParser(file));
        case ParsingType::SPELL:
            return parse_file(content, dnd::SpellParser(file));
        case ParsingType::CHOOSABLES:
            return parse_file(content, dnd::ChoosableGroupParser(file));
        default:
            assert(false);
            return dnd::Errors();
    }
}

static const char* type_directory_name(ParsingType type) {
    switch (type) {
        case ParsingType::CHARACTER:
            return "characters";
        case ParsingType::RACE:
            return "races";
        case ParsingType::CLASS:
            return "classes";
        case ParsingType::SUBRACE:
            return "subraces";
        case ParsingType::SUBCLASS:
            return "subclasses";
        case ParsingType::ITEM:
            return "items";
        case ParsingType::SPELL:
            return "spells";
        case ParsingType::CHOOSABLES:
            return "groups";
        default:
            assert(false);
            return "";
    }
}

static dnd::Errors parse_all_of_type(
    ParsingType type, dnd::Content& content, const std::vector<std::filesystem::directory_entry>& content_directories
) {
    DND_MEASURE_SCOPE(fmt::format("Parsing all: {}", type_directory_name(type)).c_str());
    dnd::Errors errors;

    const char* directory_name = type_directory_name(type);

    for (const auto& dir : content_directories) {
        const std::filesystem::directory_entry directory(dir.path() / directory_name);
        if (!std::filesystem::exists(directory)) {
            continue;
        }
        if (!std::filesystem::is_directory(directory)) {
            errors.add_parsing_error(
                dnd::ParsingErrorCode::INVALID_FILE_FORMAT, directory.path(),
                fmt::format("Expected '{}' to be a directory but it wasn't.", directory.path().string())
            );
            continue;
        }
        for (const auto& entry : std::filesystem::directory_iterator(dir.path() / directory_name)) {
            if (!entry.is_regular_file()) {
                errors.add_parsing_error(
                    dnd::ParsingErrorCode::INVALID_FILE_FORMAT, entry.path(),
                    fmt::format("Expected '{}' to be a regular file but it wasn't.", entry.path().string())
                );
                continue;
            }
            errors += parse_file_of_type(entry.path(), content, type);
        }
    }
    return errors;
}

static dnd::Errors parse_string_groups(
    dnd::Content& content, const std::vector<std::filesystem::directory_entry>& content_directories
) {
    DND_MEASURE_FUNCTION();
    dnd::Errors errors;
    for (const auto& dir : content_directories) {
        const std::filesystem::directory_entry groups_file(dir.path() / "groups.json");
        if (std::filesystem::exists(groups_file)) {
            if (std::filesystem::is_regular_file(groups_file)) {
                errors += parse_file(content, dnd::StringGroupParser(groups_file.path()));
            } else {
                errors.add_parsing_error(
                    dnd::ParsingErrorCode::INVALID_FILE_FORMAT, groups_file.path(),
                    fmt::format("Expected '{}' to be a regular file but it wasn't.", groups_file.path().string())
                );
            }
            continue;
        }
    }
    return errors;
}
