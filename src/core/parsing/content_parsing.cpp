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
#include <core/log.hpp>
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
#include <core/parsing/v2_file_parser.hpp>

namespace dnd {

static Errors parse_file(Content& content, FileParser&& parser) {
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

ParsingResult parse_content(const std::filesystem::path& content_path, const std::string& campaign_dir_name) {
    DND_MEASURE_FUNCTION();
    ParsingResult result;
    result.content_path = content_path;
    result.campaign_directory_name = campaign_dir_name;
    if (!result.errors.ok()) {
        return result;
    }

    for (const auto& dir_entry : std::filesystem::recursive_directory_iterator(content_path)) {
        if (std::filesystem::is_directory(dir_entry)) {
            continue;
        }
        result.errors += parse_file(result.content, V2FileParser(dir_entry.path()));
    }

    return result;
}


} // namespace dnd
