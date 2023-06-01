#include "dnd_config.hpp"

#include "spells_file_parser.hpp"

#include <algorithm>
#include <cctype>
#include <future>
#include <iostream>
#include <mutex>
#include <regex>
#include <string>
#include <string_view>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include "core/controllers/groups.hpp"
#include "core/models/spell.hpp"
#include "core/parsing/parsing_exceptions.hpp"
#include "core/parsing/parsing_types.hpp"

constexpr const char*
    dnd::SpellsFileParser::spell_components_regex_cstr = "((1st|2nd|3rd|[4-9]th)-level "
                                                         "([aA]bjuration|[cC]onjuration|[dD]ivination|[eE]nchantment|"
                                                         "[eE]vocation|[iI]llusion|[nN]ecromancy|[tT]ransmutation)"
                                                         "( \\(ritual\\))?)|("
                                                         "([aA]bjuration|[cC]onjuration|[dD]ivination|[eE]nchantment|"
                                                         "[eE]vocation|[iI]llusion|[nN]ecromancy|[tT]ransmutation)"
                                                         " cantrip)";

constexpr const char* dnd::SpellsFileParser::
    spell_type_regex_cstr = "(V, S, M (\\((.*)\\))|V, S|V, M (\\((.*)\\))|S, M (\\((.*)\\))|V|S|M (\\((.*)\\)))";

void dnd::SpellsFileParser::createSpell(std::string_view spell_name, const nlohmann::json& spell_json) {
    DND_MEASURE_FUNCTION();
    SpellParsingInfo info;
    info.name = spell_name;
    info.casting_time = spell_json.at("casting_time").get<std::string>();
    info.range = spell_json.at("range").get<std::string>();
    info.duration = spell_json.at("duration").get<std::string>();
    info.description = spell_json.at("description").get<std::string>();
    info.classes = spell_json.at("classes").get<std::unordered_set<std::string>>();
    info.type = createSpellType(spell_json.at("level_type").get<std::string>());
    info.components = createSpellComponents(spell_json.at("components").get<std::string>());
    std::lock_guard<std::mutex> lock(spell_parsing_mutex);
    spell_parsing_info.emplace_back(std::move(info));
}

void dnd::SpellsFileParser::parse() {
    DND_MEASURE_FUNCTION();
    if (!json_to_parse.is_object()) {
        throw json_format_error(type, filepath, "map/object");
    }
    spells_in_file = json_to_parse.size();
    spell_parsing_info.reserve(spells_in_file);

    std::vector<std::future<void>> futures;
    for (const auto& [spell_name, spell_json] : json_to_parse.items()) {
        if (spell_name.empty()) {
            throw invalid_attribute(type, filepath, "spell name", "cannot be \"\".");
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
    if (!std::regex_match(spell_type_str, spell_type_regex)) {
        // TODO: think about how to reintroduce spell name into error message
        throw attribute_type_error(type, filepath, "invalid spell type format: \"" + spell_type_str + "\"");
    }
    SpellType spell_type;
    size_t ritual_idx = spell_type_str.find(" (ritual)");
    spell_type.is_ritual = ritual_idx != std::string::npos;
    std::string magic_school_str;
    size_t cantrip_idx = spell_type_str.find(" cantrip");
    if (cantrip_idx != std::string::npos) {
        spell_type.level = SpellLevel::CANTRIP;
        magic_school_str = spell_type_str.substr(0, cantrip_idx);
    } else {
        spell_type.level = SpellLevel(std::atoi(&spell_type_str[0]));
        size_t i = spell_type_str.find("level ") + 6;
        if (spell_type.is_ritual) {
            magic_school_str = spell_type_str.substr(i, ritual_idx - i);
        } else {
            magic_school_str = spell_type_str.substr(i, spell_type_str.size() - i);
        }
    }
    auto tolower = [](unsigned char c) { return static_cast<unsigned char>(std::tolower(c)); };
    std::transform(magic_school_str.begin(), magic_school_str.end(), magic_school_str.begin(), tolower);
    spell_type.magic_school = magic_school_from_name(magic_school_str);
    return spell_type;
}

dnd::SpellComponents dnd::SpellsFileParser::createSpellComponents(const std::string& spell_components_str) const {
    DND_MEASURE_FUNCTION();
    if (!std::regex_match(spell_components_str, spell_components_regex)) {
        // TODO: think about how to reintroduce spell name into error message
        throw attribute_type_error(type, filepath, "invalid spell components format: \"" + spell_components_str + "\"");
    }
    size_t parentheses_idx = spell_components_str.find(" (");
    std::string first_part = (parentheses_idx == std::string::npos) ? spell_components_str
                                                                    : spell_components_str.substr(0, parentheses_idx);
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
        spell_components.materials_needed = spell_components_str.substr(
            parentheses_idx + 2, spell_components_str.size() - parentheses_idx - 3
        );
    }
    return spell_components;
}


bool dnd::SpellsFileParser::validate() const {
    valid.reserve(spells_in_file);
    for (const SpellParsingInfo& info : spell_parsing_info) {
        if (spells.contains(info.name)) {
            std::cerr << "Warning: Duplicate of spell \"" << info.name << "\" found.\n";
            valid.emplace_back(false);
            continue;
        }
        valid.emplace_back(true);
    }
    return std::any_of(valid.cbegin(), valid.cend(), [](bool b) { return b; });
}

static constexpr std::array<const char*, 10> level_group_names = {
    "cantrips",       "level 1 spells", "level 2 spells", "level 3 spells", "level 4 spells",
    "level 5 spells", "level 6 spells", "level 7 spells", "level 8 spells", "level 9 spells",
};

void dnd::SpellsFileParser::saveResult() {
    for (size_t i = 0; i < spells_in_file; ++i) {
        if (valid[i]) {
            SpellParsingInfo& info = spell_parsing_info[i];

            groups.add("spells", info.name);
            const std::string& level_group_name = level_group_names.at(static_cast<size_t>(info.type.level_number()));
            groups.add(level_group_name, info.name);
            for (const std::string& class_name : info.classes) {
                groups.add(class_name + " spells", info.name);
                groups.add(class_name + ' ' + level_group_name, info.name);
            }

            spells.create(
                info.name, filepath, info.type, info.casting_time, info.range, info.components, info.duration,
                info.description, info.classes
            );
        }
    }
}
