#include <dnd_config.hpp>

#include "character_subspecies_parser.hpp"

#include <filesystem>
#include <utility>

#include <nlohmann/json.hpp>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/models/character_subspecies/character_subspecies.hpp>
#include <core/parsing/effects_provider/feature_parser.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/validation/character_subspecies/character_subspecies_data.hpp>

dnd::CharacterSubspeciesParser::CharacterSubspeciesParser(const std::filesystem::path& filepath) noexcept
    : FileParser(filepath, false), feature_parser(filepath), data() {}

dnd::Errors dnd::CharacterSubspeciesParser::parse() {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(
            ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(), "The character subspecies json is not an object."
        );
        return errors;
    }

    errors += parse_required_attribute_into(json, "name", data.name);
    errors += parse_required_attribute_into(json, "description", data.description);
    data.source_path = get_filepath();

    errors += parse_required_attribute_into(json, "species", data.species_name);

    if (json.contains("features")) {
        errors += feature_parser.parse_multiple_into(std::move(json["features"]), data.features_data, &data);
    } else {
        errors.add_parsing_error(
            ParsingErrorCode::MISSING_ATTRIBUTE, get_filepath(), "Character subspecies has no features."
        );
    }

    return errors;
}

dnd::Errors dnd::CharacterSubspeciesParser::validate(const dnd::Content& content) const {
    Errors errors = data.validate();
    errors += data.validate_relations(content);
    return errors;
}

void dnd::CharacterSubspeciesParser::save_result(dnd::Content& content) {
    content.add_character_subspecies(CharacterSubspecies::create(std::move(data), content));
}
