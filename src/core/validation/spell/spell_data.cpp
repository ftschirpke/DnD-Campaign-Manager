#include <dnd_config.hpp>

#include "spell_data.hpp"

#include <regex>
#include <string>
#include <string_view>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/spell/spell_components.hpp>
#include <core/models/spell/spell_type.hpp>
#include <core/utils/string_manipulation.hpp>
#include <core/validation/validation_data.hpp>

static constexpr const char* spell_components_regex_cstr = "((1st|2nd|3rd|[4-9]th)-level "
                                                           "([aA]bjuration|[cC]onjuration|[dD]ivination|[eE]nchantment|"
                                                           "[eE]vocation|[iI]llusion|[nN]ecromancy|[tT]ransmutation)"
                                                           "( \\(ritual\\))?)|("
                                                           "([aA]bjuration|[cC]onjuration|[dD]ivination|[eE]nchantment|"
                                                           "[eE]vocation|[iI]llusion|[nN]ecromancy|[tT]ransmutation)"
                                                           " cantrip)";

static constexpr const char*
    spell_type_regex_cstr = "(V, S, M \\(.+\\)|V, S|V, M \\(.*\\)|S, M \\(.+\\)|V|S|M \\(.+\\))";

dnd::Errors dnd::SpellData::validate() const {
    DND_MEASURE_FUNCTION();
    Errors errors = ValidationData::validate();
    errors.merge(validate_components());
    errors.merge(validate_type());
    if (casting_time.empty()) {
        errors.add_validation_error(ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, "Casting time is empty");
    }
    if (range.empty()) {
        errors.add_validation_error(ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, "Range is empty");
    }
    if (duration.empty()) {
        errors.add_validation_error(ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, "Duration is empty");
    }
    return errors;
}

dnd::Errors dnd::SpellData::validate_components() const {
    DND_MEASURE_FUNCTION();
    static const std::regex spell_components_regex(spell_components_regex_cstr);
    Errors errors;
    if (!std::regex_match(components_str, spell_components_regex)) {
        errors.add_validation_error(ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, "Invalid spell components");
    }
    return errors;
}

dnd::Errors dnd::SpellData::validate_type() const {
    DND_MEASURE_FUNCTION();
    static const std::regex spell_type_regex(spell_type_regex_cstr);
    Errors errors;
    if (!std::regex_match(type_str, spell_type_regex)) {
        errors.add_validation_error(ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, "Invalid spell type");
    }
    return errors;
}

dnd::SpellComponents dnd::SpellData::create_components() const {
    DND_MEASURE_FUNCTION();
    if (!validate_components().ok()) {
        throw invalid_data("Invalid spell components");
    }
    bool verbal, somatic, material;
    std::string materials_needed;

    size_t parentheses_idx = components_str.find(" (");
    std::string first_part = (parentheses_idx == std::string::npos) ? components_str
                                                                    : components_str.substr(0, parentheses_idx);
    if (first_part.size() == 7) {
        verbal = true;
        somatic = true;
        material = true;
    } else if (first_part.size() == 4) {
        verbal = first_part[0] == 'V';
        somatic = first_part[0] == 'S' || first_part[3] == 'S';
        material = first_part[3] == 'M';
    } else if (first_part.size() == 1) {
        verbal = first_part == "V";
        somatic = first_part == "S";
        material = first_part == "M";
    }
    if (material) {
        materials_needed = components_str.substr(parentheses_idx + 2, components_str.size() - parentheses_idx - 3);
    }
    return SpellComponents(verbal, somatic, material, std::move(materials_needed));
}

dnd::SpellType dnd::SpellData::create_type() const {
    DND_MEASURE_FUNCTION();
    if (!validate_type().ok()) {
        throw invalid_data("Invalid spell type");
    }
    bool is_ritual;
    SpellLevel level;
    MagicSchool magic_school;

    size_t ritual_idx = type_str.find(" (ritual)");
    is_ritual = ritual_idx != std::string::npos;
    std::string magic_school_str;
    size_t cantrip_idx = type_str.find(" cantrip");
    if (cantrip_idx != std::string::npos) {
        level = SpellLevel::CANTRIP;
        magic_school_str = type_str.substr(0, cantrip_idx);
    } else {
        level = SpellLevel(std::atoi(&type_str[0]));
        size_t i = type_str.find("level ") + 6;
        if (is_ritual) {
            magic_school_str = type_str.substr(i, ritual_idx - i);
        } else {
            magic_school_str = type_str.substr(i, type_str.size() - i);
        }
    }
    string_to_lowercase(magic_school_str);
    magic_school = magic_school_from_name(magic_school_str);
    return SpellType(level, magic_school, is_ritual);
}
