#include <dnd_config.hpp>

#include "character_subclass_parser.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/parsing/feature/feature_parser.hpp>
#include <core/parsing/parser.hpp>
#include <core/validation/character_class/character_class_data.hpp>

dnd::CharacterSubclassParser::CharacterSubclassParser(const std::filesystem::path& filepath) noexcept
    : Parser(filepath), feature_parser(filepath) {}

dnd::Errors dnd::CharacterSubclassParser::parse(nlohmann::ordered_json&& json, CharacterSubclassData& data) const {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(
            ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(), "The character subclass json is not an object."
        );
        return errors;
    }

    errors += parse_required_attribute(json, "name", data.name);
    errors += parse_required_attribute(json, "description", data.description);
    data.source_path = get_filepath();

    errors += parse_required_attribute(json, "class", data.class_name);

    if (json.contains("features")) {
        errors += feature_parser.parse_multiple(std::move(json["features"]), data.features_data, &data);
    } else {
        errors.add_parsing_error(
            ParsingErrorCode::MISSING_ATTRIBUTE, get_filepath(), "Character subclass has no features."
        );
    }

    return errors;
}
