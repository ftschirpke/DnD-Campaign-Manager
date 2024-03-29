#include <dnd_config.hpp>

#include "class_parser.hpp"

#include <filesystem>
#include <string>
#include <utility>

#include <nlohmann/json.hpp>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/models/class/class.hpp>
#include <core/parsing/effects_provider/class_feature_parser.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/validation/class/class_validation.hpp>
#include <core/validation/spellcasting/spellcasting_validation.hpp>

namespace dnd {

ClassParser::ClassParser(const std::filesystem::path& filepath)
    : FileParser(filepath, false), class_feature_parser(filepath), data() {}

Errors ClassParser::parse() {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(
            ParsingError::Code::INVALID_FILE_FORMAT, get_filepath(), "The character class json is not an object."
        );
        return errors;
    }

    errors += parse_required_attribute_into(json, "name", data.name);
    errors += parse_required_attribute_into(json, "description", data.description);
    data.source_path = get_filepath();

    data.spellcasting_data.is_spellcaster = json.contains("spellcasting");
    if (data.spellcasting_data.is_spellcaster) {
        nlohmann::ordered_json& spellcasting_json = json["spellcasting"];
        errors += parse_required_attribute_into(spellcasting_json, "ability", data.spellcasting_data.ability);
        errors += parse_required_attribute_into(
            spellcasting_json, "ritual_casting", data.spellcasting_data.ritual_casting
        );

        errors += parse_optional_attribute_into(
            spellcasting_json, "preparation_caster", data.spellcasting_data.preparation_spellcasting_type
        );
        data.spellcasting_data.is_spells_known_type = spellcasting_json.contains("spells_known");
        errors += parse_optional_attribute_into(spellcasting_json, "spells_known", data.spellcasting_data.spells_known);

        errors += parse_optional_attribute_into(
            spellcasting_json, "cantrips_known", data.spellcasting_data.cantrips_known
        );
        errors += parse_optional_attribute_into(
            spellcasting_json, "level1_slots", data.spellcasting_data.spell_slots[0]
        );
        errors += parse_optional_attribute_into(
            spellcasting_json, "level2_slots", data.spellcasting_data.spell_slots[1]
        );
        errors += parse_optional_attribute_into(
            spellcasting_json, "level3_slots", data.spellcasting_data.spell_slots[2]
        );
        errors += parse_optional_attribute_into(
            spellcasting_json, "level4_slots", data.spellcasting_data.spell_slots[3]
        );
        errors += parse_optional_attribute_into(
            spellcasting_json, "level5_slots", data.spellcasting_data.spell_slots[4]
        );
        errors += parse_optional_attribute_into(
            spellcasting_json, "level6_slots", data.spellcasting_data.spell_slots[5]
        );
        errors += parse_optional_attribute_into(
            spellcasting_json, "level7_slots", data.spellcasting_data.spell_slots[6]
        );
        errors += parse_optional_attribute_into(
            spellcasting_json, "level8_slots", data.spellcasting_data.spell_slots[7]
        );
        errors += parse_optional_attribute_into(
            spellcasting_json, "level9_slots", data.spellcasting_data.spell_slots[8]
        );
    }

    errors += parse_required_attribute_into(json, "subclass_feature", data.subclass_feature_name);
    errors += parse_required_attribute_into(json, "hit_dice", data.hit_dice_str);
    errors += parse_required_attribute_into(json, "feat_levels", data.important_levels_data.feat_levels);

    if (json.contains("features")) {
        errors += class_feature_parser.parse_multiple_into(std::move(json["features"]), data.features_data);
    } else {
        errors.add_parsing_error(
            ParsingError::Code::MISSING_ATTRIBUTE, get_filepath(), "Character class has no features."
        );
    }

    return errors;
}

void ClassParser::save_result(Content& content) {
    content.add_class_result(Class::create_for(std::move(data), content));
}

} // namespace dnd
