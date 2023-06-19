#include <dnd_config.hpp>

#include "content_parser.hpp"

#include <filesystem>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include <core/controllers/content_holder.hpp>
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
    ParsingType type, dnd::ContentHolder& content,
    const std::vector<std::filesystem::directory_entry>& content_directories
);
static dnd::Errors parse_string_groups(
    dnd::ContentHolder& content, const std::vector<std::filesystem::directory_entry>& content_directories
);


dnd::ParsingResult dnd::ContentParser::parse(
    const std::filesystem::path& content_path, const std::string& campaign_dir_name
) {
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
    for (auto& entry : std::filesystem::directory_iterator(content_path / "general")) {
        if (entry.is_directory()) {
            content_directories.emplace_back(std::move(entry));
        }
    }
    content_directories.emplace_back(content_path / campaign_dir_name);

    {
        DND_MEASURE_SCOPE("Parsing string groups");
        result.errors += parse_string_groups(result.content, content_directories);
    }
    {
        DND_MEASURE_SCOPE("Parsing choosable groups");
        result.errors += parse_all_of_type(ParsingType::CHOOSABLES, result.content, content_directories);
    }
    {
        DND_MEASURE_SCOPE("Parsing spells");
        result.errors += parse_all_of_type(ParsingType::SPELL, result.content, content_directories);
    }
    {
        DND_MEASURE_SCOPE("Parsing items");
        result.errors += parse_all_of_type(ParsingType::ITEM, result.content, content_directories);
    }
    {
        DND_MEASURE_SCOPE("Parsing races");
        result.errors += parse_all_of_type(ParsingType::RACE, result.content, content_directories);
    }
    {
        DND_MEASURE_SCOPE("Parsing classes");
        result.errors += parse_all_of_type(ParsingType::CLASS, result.content, content_directories);
    }
    {
        DND_MEASURE_SCOPE("Parsing subraces");
        result.errors += parse_all_of_type(ParsingType::SUBRACE, result.content, content_directories);
    }
    {
        DND_MEASURE_SCOPE("Parsing subclasses");
        result.errors += parse_all_of_type(ParsingType::SUBCLASS, result.content, content_directories);
    }
    {
        DND_MEASURE_SCOPE("Parsing characters");
        result.errors += parse_all_of_type(ParsingType::CHARACTER, result.content, content_directories);
    }

    return result;
}

static dnd::Errors parse_file(dnd::ContentHolder& content, dnd::FileParser&& parser) {
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
        if (!successful) {
            return errors;
        }
    } catch (const std::exception& e) {
        errors.add_parsing_error(dnd::ParsingErrorCode::UNKNOWN_ERROR, parser.get_filepath(), e.what());
    }

    try {
        parser.set_context(content);

        dnd::Errors validation_errors = parser.validate(content);
        successful = validation_errors.ok();
        errors += std::move(validation_errors);
        if (!successful) {
            return errors;
        }

        parser.save_result(content);
    } catch (const std::exception& e) {
        errors.add_validation_error(dnd::ValidationErrorCode::UNKNOWN_ERROR, nullptr, e.what());
    }
    return errors;
}

static dnd::Errors parse_file_of_type(
    const std::filesystem::path& file, dnd::ContentHolder& content, ParsingType type
) {
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
    }
}

static dnd::Errors parse_all_of_type(
    ParsingType type, dnd::ContentHolder& content,
    const std::vector<std::filesystem::directory_entry>& content_directories
) {
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
        for (auto& entry : std::filesystem::directory_iterator(dir.path() / directory_name)) {
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
    dnd::ContentHolder& content, const std::vector<std::filesystem::directory_entry>& content_directories
) {
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
