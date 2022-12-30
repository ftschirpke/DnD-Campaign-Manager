#include "spells_file_parser.hpp"

#include <algorithm>
#include <memory>
#include <regex>
#include <string>
#include <unordered_map>

#include "models/spell.hpp"
#include "parsing/parsing_exceptions.hpp"
#include "parsing/parsing_types.hpp"

void dnd::SpellsFileParser::parse() {
    if (!json_to_parse.is_object()) {
        throw json_format_error(ParsingType::SPELLS, filename, "map/object");
    }
    spells_in_file = json_to_parse.size();
    names.reserve(spells_in_file);
    casting_times.reserve(spells_in_file);
    ranges.reserve(spells_in_file);
    durations.reserve(spells_in_file);
    descriptions.reserve(spells_in_file);
    types.reserve(spells_in_file);
    components.reserve(spells_in_file);

    for (const auto& [spell_name, spell_json] : json_to_parse.items()) {
        names.emplace_back(spell_name);
        if (spell_name.size() == 0) {
            throw invalid_attribute(ParsingType::SPELLS, filename, "spell name", "cannot be \"\".");
        }
        casting_times.emplace_back(spell_json.at("casting_time").get<std::string>());
        ranges.emplace_back(spell_json.at("range").get<std::string>());
        durations.emplace_back(spell_json.at("duration").get<std::string>());
        descriptions.emplace_back(spell_json.at("description").get<std::string>());
        types.emplace_back(createSpellType(spell_json.at("level_type").get<std::string>()));
        components.emplace_back(createSpellComponents(spell_json.at("components").get<std::string>()));
    }
}

dnd::SpellType dnd::SpellsFileParser::createSpellType(const std::string& spell_type_str) {
    const std::string magic_school_regex_str = "([aA]bjuration|[cC]onjuration|[dD]ivination|[eE]nchantment|[eE]"
                                               "vocation|[iI]llusion|[nN]ecromancy|[tT]ransmutation)";
    const std::regex spell_type_regex(
        "((1st|2nd|3rd|[4-9]th)-level " + magic_school_regex_str + "( \\(ritual\\))?)|(" + magic_school_regex_str
        + " cantrip)"
    );
    if (!std::regex_match(spell_type_str, spell_type_regex)) {
        throw std::invalid_argument("Spell type \"" + spell_type_str + "\" is of wrong format.");
    }
    SpellType spell_type;
    size_t ritual_idx = spell_type_str.find(" (ritual)");
    spell_type.is_ritual = ritual_idx != std::string::npos;
    std::string magic_school_str;
    size_t cantrip_idx = spell_type_str.find(" cantrip");
    if (cantrip_idx != std::string::npos) {
        spell_type.level = SpellLevel::CANTRIP;
        magic_school_str = std::string(spell_type_str.cbegin(), spell_type_str.cbegin() + cantrip_idx);
    } else {
        spell_type.level = SpellLevel(std::atoi(&spell_type_str[0]));
        size_t i = spell_type_str.find("level ") + 6;
        std::string::const_iterator end_it =
            spell_type.is_ritual ? spell_type_str.cbegin() + ritual_idx : spell_type_str.cend();
        magic_school_str = std::string(spell_type_str.cbegin() + i, end_it);
    }
    std::transform(magic_school_str.begin(), magic_school_str.end(), magic_school_str.begin(), ::tolower);
    spell_type.magic_school = magic_schools.at(magic_school_str);
    return spell_type;
}

dnd::SpellComponents dnd::SpellsFileParser::createSpellComponents(const std::string& spell_components_str) {
    const std::regex spell_components_regex(
        "(V, S, M (\\((.*)\\))|V, S|V, M (\\((.*)\\))|S, M (\\((.*)\\))|V|S|M (\\((.*)\\)))"
    );
    if (!std::regex_match(spell_components_str, spell_components_regex)) {
        throw std::invalid_argument("Spell components are of wrong format.");
    }
    std::string::const_iterator start = spell_components_str.cbegin();
    int parentheses_idx = spell_components_str.find(" (");
    std::string first_part =
        (parentheses_idx == std::string::npos) ? spell_components_str : std::string(start, start + parentheses_idx);
    SpellComponents spell_components;
    if (first_part.size() == 7) {
        spell_components.verbal = true;
        spell_components.somatic = true;
        spell_components.material = true;
    } else if (first_part.size() == 4) {
        spell_components.verbal = first_part[0] == 'V';
        spell_components.somatic = first_part[0] == 'S' || first_part[3] == 'S';
        spell_components.material = first_part[3] == 'M';
    } else if (first_part.size() == 1) {
        spell_components.verbal = first_part == "V";
        spell_components.somatic = first_part == "S";
        spell_components.material = first_part == "M";
    }
    if (spell_components.material) {
        spell_components.materials_needed = std::string(start + parentheses_idx + 2, spell_components_str.cend() - 1);
    }
    return spell_components;
}


bool dnd::SpellsFileParser::validate() const {
    valid.reserve(spells_in_file);
    for (const std::string& name : names) {
        if (results.find(name) != results.end()) {
            std::cerr << "Warning: Duplicate of spell \"" << name << "\" found.\n";
            valid.emplace_back(false);
            continue;
        }
        valid.emplace_back(true);
    }
    return std::any_of(valid.cbegin(), valid.cend(), [](bool b) { return b; });
}

void dnd::SpellsFileParser::saveResult() {
    for (int i = 0; i < spells_in_file; ++i) {
        if (valid[i]) {
            auto spell = std::make_unique<Spell>(
                names[i], types[i], casting_times[i], ranges[i], components[i], durations[i], descriptions[i]
            );
            results.emplace(names[i], std::move(spell));
        }
    }
}

void dnd::SpellsFileParser::reset() {
    spells_in_file = 0;
    names = {};
    casting_times = {};
    ranges = {};
    durations = {};
    descriptions = {};
    types = {};
    components = {};
    valid = {};
}
