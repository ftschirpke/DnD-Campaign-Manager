#include <dnd_config.hpp>

#include "character_subrace_parser.hpp"

#include <filesystem>
#include <utility>

#include <nlohmann/json.hpp>

#include <core/controllers/content_holder.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/models/character_subrace/character_subrace.hpp>
#include <core/parsing/feature/feature_parser.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/validation/character_subrace/character_subrace_data.hpp>

dnd::CharacterSubraceParser::CharacterSubraceParser(const std::filesystem::path& filepath) noexcept
    : FileParser(filepath), feature_parser(filepath), data() {}

dnd::Errors dnd::CharacterSubraceParser::parse() {
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

    if (json.contains("features")) {
        errors += feature_parser.parse_multiple(std::move(json["features"]), data.features_data, &data);
    } else {
        errors.add_parsing_error(
            ParsingErrorCode::MISSING_ATTRIBUTE, get_filepath(), "Character subrace has no features."
        );
    }

    return errors;
}

dnd::Errors dnd::CharacterSubraceParser::validate(const dnd::ContentHolder& content) const {
    Errors errors = data.validate();
    errors += data.validate_relations(content);
    return errors;
}

void dnd::CharacterSubraceParser::save_result(dnd::ContentHolder& content) {
    content.add_character_subrace(CharacterSubrace::create(std::move(data), content));
}
