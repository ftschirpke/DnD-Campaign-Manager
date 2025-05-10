#include <dnd_config.hpp>

#include "character_parsing.hpp"

#include <filesystem>
#include <optional>
#include <string>

#include <fmt/format.h>
#include <fmt/ranges.h>
#include <nlohmann/json.hpp>

#include <constants.hpp>
#include <core/basic_mechanics/character_progression.hpp>
#include <core/errors/errors.hpp>
#include <core/models/character/character.hpp>
#include <core/parsing/parser.hpp>

namespace dnd {

static WithErrors<Progression::Data> parse_character_progression(
    const nlohmann::json& obj, const std::filesystem::path& filepath
) {
    WithErrors<Progression::Data> result{};
    Progression::Data& progression_data = result.value;
    Errors& errors = result.errors;

    errors += check_required_attribute(obj, "progression", filepath, JsonType::OBJECT);
    if (!errors.ok()) {
        return result;
    }
    const nlohmann::json& progression = obj["progression"];

    bool has_xp = progression.contains("xp");
    bool has_level = progression.contains("level");
    errors += parse_optional_attribute_into(progression, "xp", progression_data.xp, filepath);
    errors += parse_optional_attribute_into(progression, "level", progression_data.level, filepath);
    if (has_xp && !has_level) {
        progression_data.level = level_for_xp(progression_data.xp).value_or(0);
    } else if (has_level && !has_xp) {
        progression_data.xp = xp_for_level(progression_data.level).value_or(0);
    }

    errors += parse_required_attribute_into(progression, "hdRolls", progression_data.hit_dice_rolls, filepath);

    return result;
}

static WithErrors<FeatureProviders::Data> parse_character_feature_providers(
    const nlohmann::json& obj, const std::filesystem::path& filepath
) {
    WithErrors<FeatureProviders::Data> result;
    FeatureProviders::Data& feature_providers_data = result.value;
    Errors& errors = result.errors;

    std::optional<Error> check_class_error = check_required_attribute(obj, "class", filepath, JsonType::OBJECT);
    if (check_class_error.has_value()) {
        errors += check_class_error.value();
    } else {
        const nlohmann::json& cls = obj["class"];
        std::string class_name, class_source;
        errors += parse_required_attribute_into(cls, "name", class_name, filepath);
        errors += parse_required_attribute_into(cls, "source", class_source, filepath);
        feature_providers_data.class_key = Class::key(class_name, class_source);
    }

    if (obj.contains("subclass")) {
        std::optional<Error> check_subclass_error = check_required_attribute(
            obj, "subclass", filepath, JsonType::OBJECT
        );
        if (check_subclass_error.has_value()) {
            errors += check_subclass_error.value();
        } else {
            const nlohmann::json& subclass = obj["subclass"];
            std::string subclass_short_name, subclass_source;
            errors += parse_required_attribute_into(subclass, "shortName", subclass_short_name, filepath);
            errors += parse_required_attribute_into(subclass, "source", subclass_source, filepath);
            feature_providers_data.subclass_key = Class::key(subclass_short_name, subclass_source);
        }
    }

    std::optional<Error> check_species_error = check_required_attribute(obj, "species", filepath, JsonType::OBJECT);
    if (check_species_error.has_value()) {
        errors += check_species_error.value();
    } else {
        const nlohmann::json& species = obj["species"];
        std::string species_name, species_source;
        errors += parse_required_attribute_into(species, "name", species_name, filepath);
        errors += parse_required_attribute_into(species, "source", species_source, filepath);
        feature_providers_data.species_key = Class::key(species_name, species_source);
    }

    if (obj.contains("subspecies")) {
        std::optional<Error> check_subspecies_error = check_required_attribute(
            obj, "subspecies", filepath, JsonType::OBJECT
        );
        if (check_subspecies_error.has_value()) {
            errors += check_subspecies_error.value();
        } else {
            const nlohmann::json& subspecies = obj["subspecies"];
            std::string subspecies_name, subspecies_source;
            errors += parse_required_attribute_into(subspecies, "name", subspecies_name, filepath);
            errors += parse_required_attribute_into(subspecies, "source", subspecies_source, filepath);
            feature_providers_data.subspecies_key = Class::key(subspecies_name, subspecies_source);
        }
    }

    return result;
}

static WithErrors<AbilityScores::Data> parse_character_base_scores(
    const nlohmann::json& obj, const std::filesystem::path& filepath
) {
    WithErrors<AbilityScores::Data> result;
    AbilityScores::Data& ability_scores_data = result.value;
    Errors& errors = result.errors;

    errors += check_required_attribute(obj, "baseScores", filepath, JsonType::OBJECT);
    if (!errors.ok()) {
        return result;
    }
    const nlohmann::json& base_scores = obj["baseScores"];

    errors += parse_required_attribute_into(base_scores, "str", ability_scores_data.strength, filepath);
    errors += parse_required_attribute_into(base_scores, "dex", ability_scores_data.dexterity, filepath);
    errors += parse_required_attribute_into(base_scores, "con", ability_scores_data.constitution, filepath);
    errors += parse_required_attribute_into(base_scores, "int", ability_scores_data.intelligence, filepath);
    errors += parse_required_attribute_into(base_scores, "wis", ability_scores_data.wisdom, filepath);
    errors += parse_required_attribute_into(base_scores, "cha", ability_scores_data.charisma, filepath);

    return result;
}

static WithErrors<std::vector<std::string>> parse_character_feats(
    const nlohmann::json& obj, const std::filesystem::path& filepath
) {
    WithErrors<std::vector<std::string>> result;
    std::vector<std::string>& feat_keys = result.value;
    Errors& errors = result.errors;

    if (!obj.contains("feats")) {
        return result;
    }
    errors += check_required_attribute(obj, "feats", filepath, JsonType::ARRAY);
    if (!errors.ok()) {
        return result;
    }
    const nlohmann::json& feats = obj["feats"];

    for (const nlohmann::json& feat_entry : feats) {
        if (!feat_entry.is_object()) {
            errors.add_parsing_error(
                ParsingError::Code::INVALID_ATTRIBUTE_TYPE, filepath,
                "Entries of the 'feats' array must be objects containing name and source"
            );
            continue;
        }

        std::string feat_name, feat_source;
        errors += parse_required_attribute_into(feat_entry, "name", feat_name, filepath);
        errors += parse_required_attribute_into(feat_entry, "source", feat_source, filepath);
        feat_keys.emplace_back(Choosable::key(feat_name, feat_source));
    }

    return result;
}


WithErrors<Character::Data> parse_character(const nlohmann::json& obj, const std::filesystem::path& filepath) {
    WithErrors<Character::Data> result;
    Character::Data& character_data = result.value;
    Errors& errors = result.errors;

    character_data.source_path = filepath;
    errors += parse_required_attribute_into(obj, "name", character_data.name, filepath);
    errors += parse_required_attribute_into(obj, "source", character_data.source_name, filepath);
    errors += write_formatted_text_into(obj, character_data.description, filepath);

    if (obj.contains("_meta")) {
        // HACK: add meta JSON as description as that (for now) mostly includes unsupported features and decisions
        character_data.description.parts.emplace_back(
            Paragraph{.parts = {SimpleText{.str = obj["_meta"].dump(8), .bold = false, .italic = false}}}
        );
    }

    // character_data.features_data; // TODO: do I want character features?
    // character_data.decisions_data; // TODO: implement character decisions

    parse_character_progression(obj, filepath).move_into(character_data.progression_data, errors);
    parse_character_feature_providers(obj, filepath).move_into(character_data.feature_providers_data, errors);
    parse_character_base_scores(obj, filepath).move_into(character_data.base_ability_scores_data, errors);
    parse_character_feats(obj, filepath).move_into(character_data.choosable_keys, errors);

    return result;
}

} // namespace dnd
