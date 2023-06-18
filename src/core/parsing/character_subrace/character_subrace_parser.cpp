#include <dnd_config.hpp>

#include "character_subrace_parser.hpp"

#include <filesystem>
#include <utility>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/parsing/feature/feature_parser.hpp>
#include <core/parsing/parser.hpp>
#include <core/validation/character_subrace/character_subrace_data.hpp>

dnd::CharacterSubraceParser::CharacterSubraceParser(const std::filesystem::path& filepath) noexcept
    : Parser(filepath), feature_parser(filepath) {}

dnd::Errors dnd::CharacterSubraceParser::parse(nlohmann::ordered_json&& json, CharacterSubraceData& data) const {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(
            ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(), "The character subrace json is not an object."
        );
        return errors;
    }

    errors += parse_required_attribute(json, "name", data.name);
    errors += parse_required_attribute(json, "description", data.description);
    data.source_path = get_filepath();

    errors += parse_required_attribute(json, "race", data.race_name);

    errors += feature_parser.parse_multiple(std::move(json), data.features_data, &data);
}
