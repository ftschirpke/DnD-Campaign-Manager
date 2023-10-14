#include <dnd_config.hpp>

#include "character_subclass_parser.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/models/character_subclass/character_subclass.hpp>
#include <core/parsing/effects_provider/class_feature_parser.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/validation/character_class/character_class_data.hpp>

dnd::CharacterSubclassParser::CharacterSubclassParser(const std::filesystem::path& filepath) noexcept
    : FileParser(filepath), class_feature_parser(filepath), data() {}

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

    data.spellcasting_data.is_spellcaster = json.contains("spellcasting");
    if (data.spellcasting_data.is_spellcaster) {
        nlohmann::ordered_json& spellcasting_json = json["spellcasting"];
        errors += parse_required_attribute(spellcasting_json, "ability", data.spellcasting_data.ability);
        errors += parse_required_attribute(spellcasting_json, "ritual_casting", data.spellcasting_data.ritual_casting);

        errors += parse_optional_attribute(
            spellcasting_json, "preparation_caster", data.spellcasting_data.preparation_spellcasting_type
        );
        data.spellcasting_data.is_spells_known_type = spellcasting_json.contains("spells_known");
        errors += parse_optional_attribute(spellcasting_json, "spells_known", data.spellcasting_data.spells_known);

        errors += parse_optional_attribute(spellcasting_json, "cantrips_known", data.spellcasting_data.cantrips_known);
        errors += parse_optional_attribute(spellcasting_json, "level1_slots", data.spellcasting_data.spell_slots[0]);
        errors += parse_optional_attribute(spellcasting_json, "level2_slots", data.spellcasting_data.spell_slots[1]);
        errors += parse_optional_attribute(spellcasting_json, "level3_slots", data.spellcasting_data.spell_slots[2]);
        errors += parse_optional_attribute(spellcasting_json, "level4_slots", data.spellcasting_data.spell_slots[3]);
        errors += parse_optional_attribute(spellcasting_json, "level5_slots", data.spellcasting_data.spell_slots[4]);
        errors += parse_optional_attribute(spellcasting_json, "level6_slots", data.spellcasting_data.spell_slots[5]);
        errors += parse_optional_attribute(spellcasting_json, "level7_slots", data.spellcasting_data.spell_slots[6]);
        errors += parse_optional_attribute(spellcasting_json, "level8_slots", data.spellcasting_data.spell_slots[7]);
        errors += parse_optional_attribute(spellcasting_json, "level9_slots", data.spellcasting_data.spell_slots[8]);
    }

    errors += parse_required_attribute(json, "class", data.class_name);

    if (json.contains("features")) {
        errors += class_feature_parser.parse_multiple(std::move(json["features"]), data.features_data, &data);
    } else {
        errors.add_parsing_error(
            ParsingErrorCode::MISSING_ATTRIBUTE, get_filepath(), "Character subclass has no features."
        );
    }

    return errors;
}

dnd::Errors dnd::CharacterSubclassParser::validate(const Content& content) const {
    Errors errors = data.validate();
    errors += data.validate_relations(content);
    return errors;
}

void dnd::CharacterSubclassParser::save_result(Content& content) {
    content.add_character_subclass(CharacterSubclass::create(std::move(data), content));
}
