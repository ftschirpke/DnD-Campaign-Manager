#include "spell_parser.hpp"

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <memory>
#include <regex>
#include <string>

#include <nlohmann/json.hpp>

#include "models/spell.hpp"

std::shared_ptr<const dnd::Spell> dnd::SpellParser::createSpell(
    const std::string& spell_name, const nlohmann::json& spell_json
) {
    if (!spell_json.is_object()) {
        throw std::invalid_argument("Spell \"" + spell_name + "\" is not formatted as an object/map.");
    }
    const std::string name = spell_name;
    const SpellType type = *createSpellType(spell_json.at("level_type"));
    const std::string casting_time = spell_json.at("casting_time");
    const std::string range = spell_json.at("range");
    const SpellComponents components = *createSpellComponents(spell_json.at("components"));
    const std::string duration = spell_json.at("duration");
    const std::string description = spell_json.at("description");
    return std::make_shared<const Spell>(name, type, casting_time, range, components, duration, description);
}

std::unique_ptr<dnd::SpellComponents> dnd::SpellParser::createSpellComponents(const std::string& components_str) {
    const std::regex components_regex(
        "(V, S, M (\\((.*)\\))|V, S|V, M (\\((.*)\\))|S, M (\\((.*)\\))|V|S|M (\\((.*)\\)))"
    );
    if (!std::regex_match(components_str, components_regex)) {
        throw std::invalid_argument("Spell components are of wrong format.");
    }
    std::string::const_iterator start = components_str.cbegin();
    int parentheses_idx = components_str.find(" (");
    std::string first_part =
        (parentheses_idx == std::string::npos) ? components_str : std::string(start, start + parentheses_idx);
    std::unique_ptr<SpellComponents> components_ptr(new SpellComponents);
    if (first_part.size() == 7) {
        components_ptr->verbal = true;
        components_ptr->somatic = true;
        components_ptr->material = true;
    } else if (first_part.size() == 4) {
        components_ptr->verbal = first_part[0] == 'V';
        components_ptr->somatic = first_part[0] == 'S' || first_part[3] == 'S';
        components_ptr->material = first_part[3] == 'M';
    } else if (first_part.size() == 1) {
        components_ptr->verbal = first_part == "V";
        components_ptr->somatic = first_part == "S";
        components_ptr->material = first_part == "M";
    }
    if (components_ptr->material) {
        components_ptr->materials_needed = std::string(start + parentheses_idx + 2, components_str.cend() - 1);
    }
    return components_ptr;
}

std::unique_ptr<dnd::SpellType> dnd::SpellParser::createSpellType(const std::string& type_str) {
    const std::string magic_school_regex_str = "([aA]bjuration|[cC]onjuration|[dD]ivination|[eE]nchantment|[eE]"
                                               "vocation|[iI]llusion|[nN]ecromancy|[tT]ransmutation)";
    const std::regex type_regex(
        "((1st|2nd|3rd|[4-9]th)-level " + magic_school_regex_str + "( \\(ritual\\))?)|(" + magic_school_regex_str
        + " cantrip)"
    );
    if (!std::regex_match(type_str, type_regex)) {
        throw std::invalid_argument("Spell type is of wrong format.");
    }
    std::unique_ptr<SpellType> type_ptr(new SpellType());
    size_t ritual_idx = type_str.find(" (ritual)");
    type_ptr->is_ritual = ritual_idx != std::string::npos;
    std::string magic_school_str;
    size_t cantrip_idx = type_str.find(" cantrip");
    if (cantrip_idx != std::string::npos) {
        type_ptr->level = SpellLevel::CANTRIP;
        magic_school_str = std::string(type_str.cbegin(), type_str.cbegin() + cantrip_idx);
    } else {
        type_ptr->level = SpellLevel(std::atoi(&type_str[0]));
        size_t i = type_str.find("level ") + 6;
        std::string::const_iterator end_it = type_ptr->is_ritual ? type_str.cbegin() + ritual_idx : type_str.cend();
        magic_school_str = std::string(type_str.cbegin() + i, end_it);
    }
    std::transform(magic_school_str.begin(), magic_school_str.end(), magic_school_str.begin(), ::tolower);
    type_ptr->magic_school = magic_schools.at(magic_school_str);
    return type_ptr;
}
