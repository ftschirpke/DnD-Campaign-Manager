#include <dnd_config.hpp>

#include "character_subclass_parser.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include <core/controllers/content_holder.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/models/character_subclass/character_subclass.hpp>
#include <core/parsing/feature/feature_parser.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/validation/character_class/character_class_data.hpp>

dnd::CharacterSubclassParser::CharacterSubclassParser(const std::filesystem::path& filepath) noexcept
    : FileParser(filepath), feature_parser(filepath), data() {}

dnd::Errors dnd::CharacterSubclassParser::parse() {
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

dnd::Errors dnd::CharacterSubclassParser::validate(const ContentHolder& content) const {
    Errors errors = data.validate();
    errors += data.validate_relations(content);
    return errors;
}

void dnd::CharacterSubclassParser::save_result(ContentHolder& content) {
    content.character_subclasses.add(CharacterSubclass::create(std::move(data), content));
}
