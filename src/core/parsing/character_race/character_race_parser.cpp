#include <dnd_config.hpp>

#include "character_race_parser.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/models/character_race/character_race.hpp>
#include <core/parsing/effects_provider/feature_parser.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/validation/character_race/character_race_data.hpp>

dnd::CharacterRaceParser::CharacterRaceParser(const std::filesystem::path& filepath) noexcept
    : FileParser(filepath), feature_parser(filepath), data() {}

dnd::Errors dnd::CharacterRaceParser::parse() {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(
            ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(), "The character race json is not an object."
        );
        return errors;
    }

    errors += parse_required_attribute(json, "name", data.name);
    errors += parse_required_attribute(json, "description", data.description);
    data.source_path = get_filepath();

    errors += parse_required_attribute(json, "has_subraces", data.subraces);

    if (json.contains("features")) {
        errors += feature_parser.parse_multiple(std::move(json["features"]), data.features_data, &data);
    } else {
        errors.add_parsing_error(
            ParsingErrorCode::MISSING_ATTRIBUTE, get_filepath(), "Character race has no features."
        );
    }

    return errors;
}

dnd::Errors dnd::CharacterRaceParser::validate(const dnd::Content& content) const {
    Errors errors = data.validate();
    errors += data.validate_relations(content);
    return errors;
}

void dnd::CharacterRaceParser::save_result(dnd::Content& content) {
    content.add_character_race(CharacterRace::create(std::move(data), content));
}
