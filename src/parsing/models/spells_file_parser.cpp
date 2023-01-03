#include "dnd_config.hpp"

#include "spells_file_parser.hpp"

#include <algorithm>
#include <future>
#include <memory>
#include <mutex>
#include <regex>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "models/spell.hpp"
#include "parsing/parsing_exceptions.hpp"
#include "parsing/parsing_types.hpp"


void dnd::SpellsFileParser::createSpell(std::string_view spell_name, const nlohmann::json& spell_json_ptr) {
    DND_MEASURE_FUNCTION();
    SpellParsingInfo info;
    info.name = spell_name;
    info.casting_time = spell_json_ptr.at("casting_time").get<std::string>();
    info.range = spell_json_ptr.at("range").get<std::string>();
    info.duration = spell_json_ptr.at("duration").get<std::string>();
    info.description = spell_json_ptr.at("description").get<std::string>();
    info.type = createSpellType(spell_json_ptr.at("level_type").get<std::string>());
    info.components = createSpellComponents(spell_json_ptr.at("components").get<std::string>());
    std::lock_guard<std::mutex> lock(spell_parsing_mutex);
    spell_parsing_info.emplace_back(std::move(info));
}

void dnd::SpellsFileParser::parse() {
    DND_MEASURE_FUNCTION();
    if (!json_to_parse.is_object()) {
        throw json_format_error(ParsingType::SPELL, filename, "map/object");
    }
    spells_in_file = json_to_parse.size();
    spell_parsing_info.reserve(spells_in_file);

    std::vector<std::future<void>> futures;
    for (const auto& [spell_name, spell_json] : json_to_parse.items()) {
        if (spell_name.size() == 0) {
            throw invalid_attribute(ParsingType::SPELL, filename, "spell name", "cannot be \"\".");
        }
        futures.emplace_back(
            std::async(std::launch::async, &SpellsFileParser::createSpell, this, spell_name, spell_json)
        );
    }
    for (auto& future : futures) {
        try {
            future.get();
        } catch (const parsing_error& e) {
            throw e;
        }
    }
}

dnd::SpellType dnd::SpellsFileParser::createSpellType(const std::string& spell_type_str) const {
    DND_MEASURE_FUNCTION();
    const std::string magic_school_regex_str = "([aA]bjuration|[cC]onjuration|[dD]ivination|[eE]nchantment|"
                                               "[eE]vocation|[iI]llusion|[nN]ecromancy|[tT]ransmutation)";
    const std::regex spell_type_regex(
        "((1st|2nd|3rd|[4-9]th)-level " + magic_school_regex_str + "( \\(ritual\\))?)|(" + magic_school_regex_str
        + " cantrip)"
    );
    if (!std::regex_match(spell_type_str, spell_type_regex)) {
        // TODO: think about how to reintroduce spell name into error message
        throw attribute_type_error(filename, "invalid spell type format: \"" + spell_type_str + "\"");
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

dnd::SpellComponents dnd::SpellsFileParser::createSpellComponents(const std::string& spell_components_str) const {
    DND_MEASURE_FUNCTION();
    const std::regex spell_components_regex(
        "(V, S, M (\\((.*)\\))|V, S|V, M (\\((.*)\\))|S, M (\\((.*)\\))|V|S|M (\\((.*)\\)))"
    );
    if (!std::regex_match(spell_components_str, spell_components_regex)) {
        // TODO: think about how to reintroduce spell name into error message
        throw attribute_type_error(filename, "invalid spell components format: \"" + spell_components_str + "\"");
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
    for (const SpellParsingInfo& info : spell_parsing_info) {
        if (results.find(info.name) != results.end()) {
            std::cerr << "Warning: Duplicate of spell \"" << info.name << "\" found.\n";
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
            SpellParsingInfo& info = spell_parsing_info[i];
            auto spell = std::make_unique<Spell>(
                info.name, info.type, info.casting_time, info.range, info.components, info.duration, info.description
            );
            results.emplace(info.name, std::move(spell));
        }
    }
}
