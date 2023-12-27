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
#include <core/parsing/class/class_parser.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/parsing/groups/choosable_group_parser.hpp>
#include <core/parsing/groups/string_group_parser.hpp>
#include <core/parsing/item/item_parser.hpp>
#include <core/parsing/species/species_parser.hpp>
#include <core/parsing/spell/spell_parser.hpp>
#include <core/parsing/subclass/subclass_parser.hpp>
#include <core/parsing/subspecies/subspecies_parser.hpp>

namespace dnd {

namespace {

enum class ParsingType {
    CHARACTER,
    SPECIES,
    SUBSPECIES,
    CLASS,
    SUBCLASS,
    ITEM,
    SPELL,
    CHOOSABLES,
};

} // namespace


static Errors parse_all_of_type(
    ParsingType type, Content& content, const std::vector<std::filesystem::directory_entry>& content_directories
);
static Errors parse_string_groups(
    Content& content, const std::vector<std::filesystem::directory_entry>& content_directories
);


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
    } else {
        if (!std::filesystem::exists(content_path / "general")) {
            result.errors.add_parsing_error(
                ParsingError::Code::FILE_NOT_FOUND, content_path / "general", "The general directory does not exist."
            );
        }
        if (!std::filesystem::exists(content_path / campaign_dir_name)) {
            result.errors.add_parsing_error(
                ParsingError::Code::FILE_NOT_FOUND, content_path / campaign_dir_name,
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
        DND_MEASURE_SCOPE("Parsing: items, choosable features, species, classes");
        std::future<Errors> item_future = std::async(
            std::launch::async, parse_all_of_type, ParsingType::ITEM, content_ref, content_directories_ref
        );
        std::future<Errors> choosable_future = std::async(
            std::launch::async, parse_all_of_type, ParsingType::CHOOSABLES, content_ref, content_directories_ref
        );
        std::future<Errors> species_future = std::async(
            std::launch::async, parse_all_of_type, ParsingType::SPECIES, content_ref, content_directories_ref
        );
        std::future<Errors> class_future = std::async(
            std::launch::async, parse_all_of_type, ParsingType::CLASS, content_ref, content_directories_ref
        );
        result.errors += item_future.get();
        result.errors += choosable_future.get();
        result.errors += species_future.get();
        result.errors += class_future.get();
    }
    {
        DND_MEASURE_SCOPE("Parsing: subspecies, subclasses");
        std::future<Errors> subspecies_future = std::async(
            std::launch::async, parse_all_of_type, ParsingType::SUBSPECIES, content_ref, content_directories_ref
        );
        std::future<Errors> subclass_future = std::async(
            std::launch::async, parse_all_of_type, ParsingType::SUBCLASS, content_ref, content_directories_ref
        );
        result.errors += subspecies_future.get();
        result.errors += subclass_future.get();
    }
    {
        DND_MEASURE_SCOPE("Parsing characters");
        result.errors += parse_all_of_type(ParsingType::CHARACTER, result.content, content_directories);
    }

    return result;
}

static Errors parse_file(Content& content, FileParser&& parser) {
    DND_MEASURE_SCOPE(parser.get_filepath().string().c_str());
    Errors errors;
    bool successful = true;
    try {
        Errors open_json_errors = parser.open_json();
        successful = open_json_errors.ok();
        errors += std::move(open_json_errors);
        if (!successful) {
            return errors;
        }

        Errors parse_errors = parser.parse();
        successful = parse_errors.ok();
        errors += std::move(parse_errors);
        if (!successful && !parser.continue_after_errors()) {
            return errors;
        }
    } catch (const std::exception& e) {
        errors.add_parsing_error(ParsingError::Code::UNKNOWN_ERROR, parser.get_filepath(), e.what());
        return errors;
    }

    try {
        parser.set_context(content);

        Errors validation_errors = parser.validate(content);
        successful = validation_errors.ok();
        errors += std::move(validation_errors);
        if (!successful && !parser.continue_after_errors()) {
            return errors;
        }

        parser.save_result(content);
    } catch (const std::exception& e) {
        errors.add_validation_error(ValidationError::Code::UNKNOWN_ERROR, nullptr, e.what());
    }
    return errors;
}

static Errors parse_file_of_type(const std::filesystem::path& file, Content& content, ParsingType type) {
    switch (type) {
        case ParsingType::CHARACTER:
            return parse_file(content, CharacterParser(file));
        case ParsingType::SPECIES:
            return parse_file(content, SpeciesParser(file));
        case ParsingType::CLASS:
            return parse_file(content, ClassParser(file));
        case ParsingType::SUBSPECIES:
            return parse_file(content, SubspeciesParser(file));
        case ParsingType::SUBCLASS:
            return parse_file(content, SubclassParser(file));
        case ParsingType::ITEM:
            return parse_file(content, ItemParser(file));
        case ParsingType::SPELL:
            return parse_file(content, SpellParser(file));
        case ParsingType::CHOOSABLES:
            return parse_file(content, ChoosableGroupParser(file));
        default:
            assert(false);
            return Errors();
    }
}

static const char* type_directory_name(ParsingType type) {
    switch (type) {
        case ParsingType::CHARACTER:
            return "characters";
        case ParsingType::SPECIES:
            return "species";
        case ParsingType::CLASS:
            return "classes";
        case ParsingType::SUBSPECIES:
            return "subspecies";
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

static Errors parse_all_of_type(
    ParsingType type, Content& content, const std::vector<std::filesystem::directory_entry>& content_directories
) {
    DND_MEASURE_SCOPE(fmt::format("Parsing all: {}", type_directory_name(type)).c_str());
    Errors errors;

    const char* directory_name = type_directory_name(type);

    for (const auto& dir : content_directories) {
        const std::filesystem::directory_entry directory(dir.path() / directory_name);
        if (!std::filesystem::exists(directory)) {
            continue;
        }
        if (!std::filesystem::is_directory(directory)) {
            errors.add_parsing_error(
                ParsingError::Code::INVALID_FILE_FORMAT, directory.path(),
                fmt::format("Expected '{}' to be a directory but it wasn't.", directory.path().string())
            );
            continue;
        }
        for (const auto& entry : std::filesystem::directory_iterator(dir.path() / directory_name)) {
            if (!entry.is_regular_file()) {
                errors.add_parsing_error(
                    ParsingError::Code::INVALID_FILE_FORMAT, entry.path(),
                    fmt::format("Expected '{}' to be a regular file but it wasn't.", entry.path().string())
                );
                continue;
            }
            errors += parse_file_of_type(entry.path(), content, type);
        }
    }
    return errors;
}

static Errors parse_string_groups(
    Content& content, const std::vector<std::filesystem::directory_entry>& content_directories
) {
    DND_MEASURE_FUNCTION();
    Errors errors;
    for (const auto& dir : content_directories) {
        const std::filesystem::directory_entry groups_file(dir.path() / "groups.json");
        if (std::filesystem::exists(groups_file)) {
            if (std::filesystem::is_regular_file(groups_file)) {
                errors += parse_file(content, StringGroupParser(groups_file.path()));
            } else {
                errors.add_parsing_error(
                    ParsingError::Code::INVALID_FILE_FORMAT, groups_file.path(),
                    fmt::format("Expected '{}' to be a regular file but it wasn't.", groups_file.path().string())
                );
            }
            continue;
        }
    }
    return errors;
}

} // namespace dnd
