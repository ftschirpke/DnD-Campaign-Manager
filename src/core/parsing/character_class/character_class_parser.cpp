#include <dnd_config.hpp>

#include "character_class_parser.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include <core/controllers/content_holder.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/models/character_class/character_class.hpp>
#include <core/parsing/feature/feature_parser.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/validation/character_class/character_class_data.hpp>

dnd::CharacterClassParser::CharacterClassParser(const std::filesystem::path& filepath) noexcept
    : FileParser(filepath), feature_parser(filepath), data() {}

dnd::Errors dnd::CharacterClassParser::parse() {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(
            ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(), "The character class json is not an object."
        );
        return errors;
    }

    errors += parse_required_attribute(json, "name", data.name);
    errors += parse_required_attribute(json, "description", data.description);
    data.source_path = get_filepath();

    errors += parse_required_attribute(json, "subclass", data.subclass_feature_name);
    errors += parse_required_attribute(json, "hit_dice", data.hit_dice_data.str);
    errors += parse_required_attribute(json, "asi_levels", data.important_levels_data.asi_levels);

    if (json.contains("features")) {
        errors += feature_parser.parse_multiple(std::move(json["features"]), data.features_data, &data);
    } else {
        errors.add_parsing_error(
            ParsingErrorCode::MISSING_ATTRIBUTE, get_filepath(), "Character class has no features."
        );
    }

    return errors;
}

dnd::Errors dnd::CharacterClassParser::validate(const dnd::ContentHolder& content) const {
    Errors errors = data.validate();
    errors += data.validate_relations(content);
    return errors;
}

void dnd::CharacterClassParser::save_result(dnd::ContentHolder& content) {
    content.character_classes.add(CharacterClass::create(std::move(data), content));
}
