#include <dnd_config.hpp>

#include "class_parsing.hpp"

#include <filesystem>
#include <optional>

#include <fmt/format.h>
#include <fmt/ranges.h>
#include <nlohmann/json.hpp>

#include <constants.hpp>
#include <core/errors/errors.hpp>
#include <core/models/class/class.hpp>
#include <core/models/effects_provider/class_feature.hpp>
#include <core/parsing/parser.hpp>
#include <core/utils/rich_text.hpp>

namespace dnd {

static WithErrors<std::array<std::array<int, MAX_CHARACTER_LEVEL>, MAX_SPELL_LEVEL>>
parse_heterogeneous_spell_slots_from_table(
    const nlohmann::ordered_json& table_group, const std::filesystem::path& filepath
) {
    WithErrors<std::array<std::array<int, MAX_CHARACTER_LEVEL>, MAX_SPELL_LEVEL>> result{};
    std::array<std::array<int, MAX_CHARACTER_LEVEL>, MAX_SPELL_LEVEL>& spell_slots = result.value;
    Errors& errors = result.errors;

    if (!table_group.contains("rowsSpellProgression")) {
        errors.add_parsing_error(
            ParsingError::Code::MISSING_ATTRIBUTE, filepath,
            "Spell slots table requires 'rowsSpellProgression' attribute"
        );
        return result;
    }
    const nlohmann::ordered_json& rows = table_group["rowsSpellProgression"];
    if (!rows.is_array()) {
        errors.add_parsing_error(
            ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath,
            "Spell slot table 'rowsSpellProgression' must be an array"
        );
        return result;
    }
    if (rows.size() != MAX_CHARACTER_LEVEL) {
        static_assert(MAX_CHARACTER_LEVEL == 20);
        static_assert(MAX_SPELL_LEVEL == 9);
        errors.add_parsing_error(
            ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath,
            "'rowsSpellProgression'-array in spell slot table must contain 20 integer arrays of length <= 9"
        );
    }
    for (size_t character_lv = 0; character_lv < MAX_CHARACTER_LEVEL; ++character_lv) {
        std::vector<int> spell_slot_buffer;
        errors += parse_required_index_into(rows, character_lv, spell_slot_buffer, filepath);
        if (spell_slot_buffer.size() > MAX_SPELL_LEVEL) {
            errors.add_parsing_error(
                ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath,
                "'rowsSpellProgression' must contain arrays of length <= 9"
            );
            continue;
        }
        for (size_t spell_lv = 0; spell_lv < MAX_SPELL_LEVEL; ++spell_lv) {
            spell_slots[spell_lv][character_lv] = (spell_lv < spell_slot_buffer.size()) ? spell_slot_buffer[spell_lv]
                                                                                        : 0;
        }
    }

    return result;
}

static WithErrors<std::array<std::array<int, MAX_CHARACTER_LEVEL>, MAX_SPELL_LEVEL>>
parse_homogeneous_spell_slots_from_table(
    const nlohmann::ordered_json& table_group, const std::filesystem::path& filepath
) {
    WithErrors<std::array<std::array<int, MAX_CHARACTER_LEVEL>, MAX_SPELL_LEVEL>> result{};
    std::array<std::array<int, MAX_CHARACTER_LEVEL>, MAX_SPELL_LEVEL>& spell_slots = result.value;
    Errors& errors = result.errors;

    size_t slot_count_idx = 0;
    size_t slot_level_idx = 0;
    bool found_slot_count = false;
    bool found_slot_level = false;

    const nlohmann::ordered_json& col_labels = table_group["colLabels"];
    size_t len = col_labels.size();
    std::string label;
    for (size_t i = 0; i < len; i++) {
        const nlohmann::ordered_json& label_json = col_labels[i];
        if (!label_json.is_string()) {
            errors.add_parsing_error(
                ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath, "Column labels must be strings"
            );
            break;
        }
        label = label_json.get<std::string>();
        if (label == "Spell Slots") {
            slot_count_idx = i;
            found_slot_count = true;
        } else if (label == "Slot Level") {
            slot_level_idx = i;
            found_slot_level = true;
        }
    }

    if (!(found_slot_count && found_slot_level)) {
        errors.add_parsing_error(
            ParsingError::Code::MISSING_ATTRIBUTE, filepath,
            "Table for homogeneous spell slots must contain 'Spell Slots' and 'Slot Level' columns"
        );
        return result;
    }
    if (!table_group.contains("rows")) {
        errors.add_parsing_error(
            ParsingError::Code::MISSING_ATTRIBUTE, filepath, "Table group must have 'rows' array of length 20"
        );
        return result;
    }
    const nlohmann::ordered_json& rows = table_group["rows"];
    if (!rows.is_array() || rows.size() != 20) {
        errors.add_parsing_error(
            ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath,
            "Table's 'rows' array must be integer array of length 20"
        );
        return result;
    }
    for (size_t i = 0; i < 20; i++) {
        if (!rows[i].is_array() || rows[i].size() != len) {
            errors.add_parsing_error(
                ParsingError::Code::MISSING_ATTRIBUTE, filepath,
                fmt::format("Table's 'rows'-array must contain arrays of length {}", len)
            );
            continue;
        }
        int slot_count;
        errors += parse_required_index_into(rows[i], slot_count_idx, slot_count, filepath);

        std::optional<Error> index_check_error = check_required_index(rows[i], slot_level_idx, filepath, JsonType::ANY);
        if (index_check_error.has_value()) {
            errors += index_check_error.value();
            continue;
        }

        size_t slot_level;
        std::string slot_level_rich_str;

        if (rows[i][slot_level_idx].is_string()) {
            errors += parse_required_index_into(rows[i], slot_level_idx, slot_level_rich_str, filepath);
            std::optional<RichText> slot_level_rich_text = parse_rich_text(slot_level_rich_str);
            if (!slot_level_rich_text.has_value()) {
                errors.add_parsing_error(
                    ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath,
                    fmt::format("Expected rich text string in 'Slot Level' column")
                );
                continue;
            }
            const std::string& slot_level_str = slot_level_rich_text.value().text;
            if (slot_level_str.empty()) {
                errors.add_parsing_error(
                    ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath,
                    fmt::format("Expected rich text string in 'Slot Level' column of length > 0")
                );
                continue;
            }
            slot_level = slot_level_str[0] - '1';
        } else {
            errors += parse_required_index_into(rows[i], slot_level_idx, slot_level, filepath);
        }

        for (size_t lv = 0; lv < MAX_SPELL_LEVEL; lv++) {
            spell_slots[lv][i] = (lv == slot_level) ? slot_count : 0;
        }
    }

    return result;
}

static WithErrors<std::array<std::array<int, MAX_CHARACTER_LEVEL>, MAX_SPELL_LEVEL>> parse_spell_slots(
    const nlohmann::ordered_json& obj, const std::filesystem::path& filepath, bool is_required
) {
    WithErrors<std::array<std::array<int, MAX_CHARACTER_LEVEL>, MAX_SPELL_LEVEL>> result{};
    std::array<std::array<int, MAX_CHARACTER_LEVEL>, MAX_SPELL_LEVEL>& spell_slots = result.value;
    Errors& errors = result.errors;

    std::optional<Error> table_groups_check_error = check_required_attribute(
        obj, "classTableGroups", filepath, JsonType::ARRAY
    );
    if (table_groups_check_error.has_value()) {
        if (is_required) {
            errors += table_groups_check_error.value();
        }
        return result;
    }

    const nlohmann::ordered_json& table_groups = obj["classTableGroups"];

    if (!table_groups.is_array()) {
        errors.add_parsing_error(
            ParsingError::Code::INVALID_FILE_FORMAT, filepath, fmt::format("'classTableGroups'-entry is not an array.")
        );
        return result;
    }

    bool done = false;
    for (const nlohmann::ordered_json& table_group : table_groups) {
        if (!table_group.is_object()) {
            continue;
        }

        if (table_group.contains("title") && table_group["title"] == "Spell Slots per Spell Level") {
            parse_heterogeneous_spell_slots_from_table(table_group, filepath).move_into(spell_slots, errors);
            done = true;
        } else if (table_group.contains("colLabels") && table_group["colLabels"].is_array()) {
            const nlohmann::ordered_json& column_labels = table_group["colLabels"];

            bool is_homogeneous_spell_slot_table = false;
            for (const nlohmann::ordered_json& column_label : column_labels) {
                assert(column_label.is_string());
                std::string label = column_label.get<std::string>();
                if (label == "Spell Slots" || label == "Slot Level") {
                    is_homogeneous_spell_slot_table = true;
                    break;
                }
            }

            if (is_homogeneous_spell_slot_table) {
                parse_homogeneous_spell_slots_from_table(table_group, filepath).move_into(spell_slots, errors);
                done = true;
            }
        }

        if (done) {
            break;
        }
    }

    if (!done && is_required) {
        errors.add_parsing_error(
            ParsingError::Code::MISSING_ATTRIBUTE, filepath, "Could not find spell slot information"
        );
    }

    return result;
}

static WithErrors<Spellcasting::Data> parse_spellcasting(
    const nlohmann::ordered_json& obj, const std::filesystem::path& filepath, bool is_subclass
) {
    WithErrors<Spellcasting::Data> result{};
    Spellcasting::Data& spellcasting_data = result.value;
    Errors& errors = result.errors;

    spellcasting_data.is_spellcaster = obj.contains("spellcastingAbility");
    if (!spellcasting_data.is_spellcaster) {
        return result;
    }

    errors += parse_required_attribute_into(obj, "spellcastingAbility", spellcasting_data.ability, filepath);
    errors += parse_attribute_into(
        obj, "casterProgression", spellcasting_data.preparation_spellcasting_type, filepath, !is_subclass
    );
    errors += parse_optional_attribute_into(obj, "cantripProgression", spellcasting_data.cantrips_known, filepath);
    errors += parse_optional_attribute_into(obj, "spellsKnownProgression", spellcasting_data.spells_known, filepath);
    if (obj.contains("spellsKnownProgression")) {
        spellcasting_data.is_spells_known_type = true;
        spellcasting_data.preparation_spellcasting_type = "";
    } else {
        spellcasting_data.is_spells_known_type = false;
        errors += parse_attribute_into(
            obj, "casterProgression", spellcasting_data.preparation_spellcasting_type, filepath, !is_subclass
        );
        if (spellcasting_data.preparation_spellcasting_type == "1/2"
            || spellcasting_data.preparation_spellcasting_type == "artificer"
            || spellcasting_data.preparation_spellcasting_type == "pact") {
            // HACK: hard-code transformations to "half" until better implementation found
            spellcasting_data.preparation_spellcasting_type = "half";
        }
        if (spellcasting_data.preparation_spellcasting_type == "1/3"
            || spellcasting_data.preparation_spellcasting_type == "") {
            // HACK: hard-code transformations to "half" until better implementation found
            spellcasting_data.preparation_spellcasting_type = "subclass";
        }
    }

    parse_spell_slots(obj, filepath, !is_subclass).move_into(spellcasting_data.spell_slots, errors);

    return result;
}

WithErrors<Class::Data> parse_class(const nlohmann::ordered_json& obj, const std::filesystem::path& filepath) {
    WithErrors<Class::Data> result;
    Class::Data& class_data = result.value;
    Errors& errors = result.errors;

    class_data.source_path = filepath;
    errors += parse_required_attribute_into(obj, "name", class_data.name, filepath);
    errors += parse_required_attribute_into(obj, "source", class_data.source_name, filepath);
    std::optional<Error> hit_dice_check_error = check_required_attribute(obj, "hd", filepath, JsonType::OBJECT);
    if (hit_dice_check_error.has_value()) {
        errors += hit_dice_check_error.value();
    } else {
        int hit_dice_number, hit_dice_faces;
        errors += parse_required_attribute_into(obj["hd"], "number", hit_dice_number, filepath);
        errors += parse_required_attribute_into(obj["hd"], "faces", hit_dice_faces, filepath);
        class_data.hit_dice_str = fmt::format("{}d{}", hit_dice_number, hit_dice_faces);
    }

    parse_spellcasting(obj, filepath, false).move_into(class_data.spellcasting_data, errors);

    return result;
}

Errors parse_class_feature(
    const nlohmann::ordered_json& obj, const std::filesystem::path& filepath,
    std::map<std::string, Class::Data>& parsed_classes
) {
    Errors errors;

    std::string class_name, class_source_name;
    errors += parse_required_attribute_into(obj, "className", class_name, filepath);
    errors += parse_required_attribute_into(obj, "classSource", class_source_name, filepath);
    std::string key = Class::key(class_name, class_source_name);

    if (!parsed_classes.contains(key)) {
        errors.add_parsing_error(
            ParsingError::Code::UNEXPECTED_ATTRIBUTE, filepath,
            fmt::format("Found class feature for '{}' but the class does not exist", key)
        );
        return errors;
    }
    Class::Data& class_data = parsed_classes.at(key);
    ClassFeature::Data& feature_data = class_data.features_data.emplace_back();

    feature_data.source_path = filepath;
    errors += parse_required_attribute_into(obj, "name", feature_data.name, filepath);
    errors += parse_required_attribute_into(obj, "source", feature_data.source_name, filepath);
    errors += parse_required_attribute_into(obj, "level", feature_data.level, filepath);
    errors += write_formatted_description_into(obj, feature_data.description, filepath);

    return errors;
}

WithErrors<Subclass::Data> parse_subclass(const nlohmann::ordered_json& obj, const std::filesystem::path& filepath) {
    WithErrors<Subclass::Data> result;
    Subclass::Data& subclass_data = result.value;
    Errors& errors = result.errors;

    subclass_data.source_path = filepath;
    errors += parse_required_attribute_into(obj, "name", subclass_data.name, filepath);
    errors += parse_required_attribute_into(obj, "source", subclass_data.source_name, filepath);
    errors += parse_required_attribute_into(obj, "shortName", subclass_data.short_name, filepath);

    std::string class_name, class_source_name;
    errors += parse_required_attribute_into(obj, "className", class_name, filepath);
    errors += parse_required_attribute_into(obj, "classSource", class_source_name, filepath);
    subclass_data.class_key = Class::key(class_name, class_source_name);

    parse_spellcasting(obj, filepath, true).move_into(subclass_data.spellcasting_data, errors);

    return result;
}

Errors parse_subclass_feature(
    const nlohmann::ordered_json& obj, const std::filesystem::path& filepath,
    std::map<std::string, Subclass::Data>& parsed_subclasses
) {
    Errors errors;

    std::string subclass_short_name, subclass_source_name;
    errors += parse_required_attribute_into(obj, "subclassShortName", subclass_short_name, filepath);
    errors += parse_required_attribute_into(obj, "subclassSource", subclass_source_name, filepath);
    std::string key = Subclass::key(subclass_short_name, subclass_source_name);

    if (!parsed_subclasses.contains(key)) {
        errors.add_parsing_error(
            ParsingError::Code::UNEXPECTED_ATTRIBUTE, filepath,
            fmt::format("Found subclass feature for '{}' but the subclass does not exist", key)
        );
        return errors;
    }
    Subclass::Data& subclass_data = parsed_subclasses.at(key);
    ClassFeature::Data& feature_data = subclass_data.features_data.emplace_back();

    feature_data.source_path = filepath;
    errors += parse_required_attribute_into(obj, "name", feature_data.name, filepath);
    errors += parse_required_attribute_into(obj, "source", feature_data.source_name, filepath);
    errors += parse_required_attribute_into(obj, "level", feature_data.level, filepath);
    if (obj.contains("_copy")) {
        // TODO: handle this case more gracefully
        std::string name, source, class_name, class_source, subclass_short_name, subclass_source;
        errors += parse_required_attribute_into(obj, "name", name, filepath);
        errors += parse_required_attribute_into(obj, "source", source, filepath);
        errors += parse_required_attribute_into(obj, "className", class_name, filepath);
        errors += parse_required_attribute_into(obj, "classSource", class_source, filepath);
        errors += parse_required_attribute_into(obj, "subclassShortName", subclass_short_name, filepath);
        errors += parse_required_attribute_into(obj, "subclassSource", subclass_source, filepath);
        feature_data.description = fmt::format(
            "Copy of {}-{} of {}-{} ({}-{})", name, source, subclass_short_name, subclass_source, class_name,
            class_source
        );
    } else {
        errors += write_formatted_description_into(obj, feature_data.description, filepath);
    }

    return errors;
}

} // namespace dnd
