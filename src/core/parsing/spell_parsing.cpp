#include <dnd_config.hpp>

#include "spell_parsing.hpp"

#include <filesystem>
#include <string>

#include <fmt/format.h>
#include <fmt/ranges.h>
#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/models/spell/spell.hpp>
#include <core/models/spell/spell_components.hpp>
#include <core/models/spell/spell_type.hpp>
#include <core/parsing/parser.hpp>
#include <core/types.hpp>

namespace dnd {

static WithErrors<std::string> parse_spell_range(const nlohmann::json& obj, const std::filesystem::path& filepath) {
    WithErrors<std::string> result;
    std::string& out = result.value;
    Errors& errors = result.errors;

    errors += check_required_attribute(obj, "range", filepath, JsonType::OBJECT);
    if (!errors.ok()) {
        return result;
    }
    const nlohmann::json& range = obj["range"];

    std::string range_type;
    errors += parse_required_attribute_into(range, "type", range_type, filepath);
    if (!errors.ok()) {
        return result;
    }
    if (range_type == "special") {
        out = "special (read description carefully)";
        return result;
    }

    errors += check_required_attribute(range, "distance", filepath, JsonType::OBJECT);
    if (!errors.ok()) {
        return result;
    }
    const nlohmann::json& distance = range["distance"];

    std::string distance_type;
    errors += parse_required_attribute_into(distance, "type", distance_type, filepath);
    if (!errors.ok()) {
        return result;
    }

    if (distance.contains("amount")) {
        int distance_amount;
        errors += parse_required_attribute_into(distance, "amount", distance_amount, filepath);
        out = fmt::format("{} {}", distance_amount, distance_type);
    } else {
        out = distance_type;
    }

    return result;
}

static WithErrors<std::pair<std::string, bool>> parse_spell_duration(
    const nlohmann::json& obj, const std::filesystem::path& filepath
) {
    WithErrors<std::pair<std::string, bool>> result;
    std::string& duration_out = result.value.first;
    bool& concentration_out = result.value.second;
    Errors& errors = result.errors;

    errors += check_required_attribute(obj, "duration", filepath, JsonType::ARRAY);
    if (!errors.ok()) {
        return result;
    }
    const nlohmann::json& duration_arr = obj["duration"];

    errors += check_required_index(duration_arr, 0, filepath, JsonType::OBJECT);
    if (!errors.ok()) {
        return result;
    }
    if (duration_arr.empty()) {
        errors.add_parsing_error(
            ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath, "Spell's 'duration' array cannot be empty"
        );
        return result;
    }


    std::optional<bool> concentration_check = std::nullopt;
    std::vector<std::string> durations;
    for (const nlohmann::json& duration : duration_arr) {
        std::string duration_type;
        errors += parse_required_attribute_into(duration, "type", duration_type, filepath);
        if (!errors.ok()) {
            return result;
        }
        if (duration_type != "timed") {
            duration_out = duration_type;
            return result;
        }

        errors += check_required_attribute(duration, "duration", filepath, JsonType::OBJECT);
        if (!errors.ok()) {
            return result;
        }
        const nlohmann::json& duration_time = duration["duration"];

        std::string time_type;
        int time_amount;
        errors += parse_required_attribute_into(duration_time, "type", time_type, filepath);
        errors += parse_required_attribute_into(duration_time, "amount", time_amount, filepath);
        if (time_amount > 1) {
            time_type += "s";
        }
        durations.push_back(fmt::format("{} {}", time_amount, time_type));

        bool this_concentration;
        errors += parse_optional_attribute_into(duration, "concentration", this_concentration, filepath);
        if (concentration_check.has_value()) {
            errors.add_parsing_error(
                ParsingError::Code::INVALID_FILE_FORMAT, filepath,
                "Expected the concentration of all spell duration options to be identical"
            );
        } else {
            concentration_check = this_concentration;
        }
    }

    duration_out = fmt::format("{}", fmt::join(durations.begin(), durations.end(), " / "));
    concentration_out = concentration_check.value_or(false);

    return result;
}

static WithErrors<std::string> parse_spell_casting_time(
    const nlohmann::ordered_json& obj, const std::filesystem::path& filepath
) {
    WithErrors<std::string> result;
    std::string& out = result.value;
    Errors& errors = result.errors;

    errors += check_required_attribute(obj, "time", filepath, JsonType::ARRAY);
    if (!errors.ok()) {
        return result;
    }
    const nlohmann::json& time_arr = obj["time"];

    errors += check_required_index(time_arr, 0, filepath, JsonType::OBJECT);
    if (!errors.ok()) {
        return result;
    }
    if (time_arr.empty()) {
        errors.add_parsing_error(
            ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath, "Spell's 'time' array cannot be empty"
        );
        return result;
    }

    std::vector<std::string> times;
    for (const nlohmann::json& time : time_arr) {
        int number;
        errors += parse_required_attribute_into(time, "number", number, filepath);
        std::string unit;
        errors += parse_required_attribute_into(time, "unit", unit, filepath);
        if (unit == "bonus") {
            unit = "bonus action";
        }
        if (number > 1) {
            unit += "s";
        }

        std::string time_str;
        if (time.contains("condition")) {
            std::string condition;
            errors += parse_required_attribute_into(time, "condition", condition, filepath);
            time_str = fmt::format("{} {}, {}", number, unit, condition);
        } else {
            time_str = fmt::format("{} {}", number, unit);
        }

        std::string time_note;
        errors += parse_optional_attribute_into(time, "note", time_note, filepath);
        if (!time_note.empty()) {
            time_str = fmt::format("{} ({})", time_str, time_note);
        }
        times.push_back(time_str);
    }

    out = fmt::format("{}", fmt::join(times.begin(), times.end(), " / "));

    return result;
}

static WithErrors<SpellComponents::Data> parse_spell_components(
    const nlohmann::ordered_json& obj, const std::filesystem::path& filepath
) {
    WithErrors<SpellComponents::Data> result{};
    SpellComponents::Data& components_data = result.value;
    Errors& errors = result.errors;

    errors += check_required_attribute(obj, "components", filepath, JsonType::OBJECT);
    if (!errors.ok()) {
        return result;
    }
    const nlohmann::json& components = obj["components"];

    errors += parse_optional_attribute_into(components, "v", components_data.verbal, filepath);
    errors += parse_optional_attribute_into(components, "s", components_data.somatic, filepath);

    if (components.contains("m") && components["m"].is_object()) {
        errors += parse_required_attribute_into(components["m"], "text", components_data.material_components, filepath);
    } else {
        errors += parse_optional_attribute_into(components, "m", components_data.material_components, filepath);
    }

    return result;
}

static WithErrors<SpellType::Data> parse_spell_type(
    const nlohmann::ordered_json& obj, const std::filesystem::path& filepath
) {
    WithErrors<SpellType::Data> result{};
    SpellType::Data& type_data = result.value;
    Errors& errors = result.errors;

    errors += parse_required_attribute_into(obj, "level", type_data.level, filepath);
    std::string magic_school_str;
    errors += parse_required_attribute_into(obj, "school", magic_school_str, filepath);
    if (magic_school_str.size() == 1) {
        type_data.magic_school_char = magic_school_str[0];
    } else {
        errors.add_parsing_error(
            ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath,
            "Spell 'school' must be a single character representing a magic school."
        );
    }

    if (obj.contains("meta") && obj["meta"].is_object()) {
        errors += parse_optional_attribute_into(obj["meta"], "ritual", type_data.ritual, filepath);
    }

    return result;
}

static Errors parse_higher_level_text_into(
    const nlohmann::ordered_json& obj, Text& out, const std::filesystem::path& filepath
) {
    Errors errors;

    if (!obj.contains("entriesHigherLevel")) {
        return errors;
    }

    CRef<nlohmann::ordered_json> higher_level_entries = obj["entriesHigherLevel"];
    if (higher_level_entries.get().is_array() && higher_level_entries.get().size() == 1) {
        higher_level_entries = higher_level_entries.get()[0];
    }

    if (!higher_level_entries.get().is_object()) {
        errors.add_parsing_error(
            ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath, "entriesHigherLevel must be an object"
        );
        return errors;
    }

    std::string type;
    errors += parse_required_attribute_into(higher_level_entries.get(), "type", type, filepath);
    if (type != "entries") {
        errors.add_parsing_error(
            ParsingError::Code::UNEXPECTED_ATTRIBUTE, filepath, "entriesHigherLevel must be of type 'entries'"
        );
    }

    std::string name;
    errors += parse_required_attribute_into(higher_level_entries.get(), "name", name, filepath);
    if (name.empty()) {
        name = "At Higher Levels";
    }
    Paragraph first_paragraph;
    first_paragraph.parts.push_back(SimpleText{.str = name + ". ", .bold = true, .italic = false});

    errors += check_required_attribute(higher_level_entries.get(), "entries", filepath, JsonType::ARRAY);
    const nlohmann::ordered_json& entries = higher_level_entries.get()["entries"];
    std::string entry;
    errors += parse_required_index_into(entries, 0, entry, filepath);
    errors += parse_paragraph(std::move(entry), first_paragraph, filepath);

    TextObject paragraph = std::move(first_paragraph);
    out.parts.push_back(paragraph);

    if (entries.size() > 1) {
        errors += write_formatted_text_into(higher_level_entries.get(), out, filepath, true);
    }

    return errors;
}


WithErrors<Spell::Data> parse_spell(
    const nlohmann::ordered_json& obj, const std::filesystem::path& filepath, const FoundryFileParser& foundry_parser
) {
    WithErrors<Spell::Data> result;
    Spell::Data& spell_data = result.value;
    Errors& errors = result.errors;

    spell_data.source_path = filepath;
    errors += parse_required_attribute_into(obj, "name", spell_data.name, filepath);
    errors += parse_required_attribute_into(obj, "source", spell_data.source_name, filepath);
    errors += write_formatted_text_into(obj, spell_data.description, filepath);
    errors += parse_higher_level_text_into(obj, spell_data.description, filepath);

    parse_spell_components(obj, filepath).move_into(spell_data.components_data, errors);
    parse_spell_type(obj, filepath).move_into(spell_data.type_data, errors);
    parse_spell_casting_time(obj, filepath).move_into(spell_data.casting_time, errors);
    parse_spell_range(obj, filepath).move_into(spell_data.range, errors);

    WithErrors<std::pair<std::string, bool>> duration_result = parse_spell_duration(obj, filepath);
    errors += std::move(duration_result.errors);
    spell_data.duration = std::move(duration_result.value.first);
    spell_data.concentration = duration_result.value.second;

    DND_UNUSED(foundry_parser); // TODO: implement

    return result;
}

} // namespace dnd
