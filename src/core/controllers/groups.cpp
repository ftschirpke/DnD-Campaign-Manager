#include <dnd_config.hpp>

#include "groups.hpp"

#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include <core/basic_mechanics/abilities.hpp>
#include <core/basic_mechanics/skills.hpp>
#include <core/models/feature/choosable_feature.hpp>
#include <core/utils/string_manipulation.hpp>

dnd::Groups::Groups() {
    data["abilities"] = std::unordered_set<std::string>(
        ability_cstrings_inorder.begin(), ability_cstrings_inorder.end()
    );

    std::string lowercase_skill;
    for (const auto& [skill, _] : skill_abilities) {
        lowercase_skill = string_lowercase_copy(skill);

        size_t idx = lowercase_skill.find('_');
        while (idx != std::string::npos) {
            lowercase_skill[idx] = ' ';
            idx = lowercase_skill.find('_');
        }
        data["skills"].insert(lowercase_skill);
    }
}

const std::unordered_map<std::string, dnd::ChoosableFeature>& dnd::Groups::get_choosable_group(
    const std::string& group_name
) const {
    return choosables.at(group_name);
}

const std::unordered_set<std::string>& dnd::Groups::get_string_group(const std::string& group_name) const {
    return data.at(group_name);
}

const std::unordered_map<std::string, std::unordered_map<std::string, dnd::ChoosableFeature>>& dnd::Groups::
    get_all_choosable_groups() const {
    return choosables;
}

const std::unordered_map<std::string, std::unordered_set<std::string>>& dnd::Groups::get_all_string_groups() const {
    return data;
}

void dnd::Groups::add(const std::string& group_name, const std::string& value) { data[group_name].insert(value); }

void dnd::Groups::add(const std::string& group_name, std::unordered_set<std::string>&& values) {
    data[group_name].insert(std::make_move_iterator(values.begin()), std::make_move_iterator(values.end()));
}

void dnd::Groups::add(const std::string& group_name, ChoosableFeature&& value) {
    choosables[group_name].emplace(value.get_name(), std::move(value));
}

void dnd::Groups::add(const std::string& group_name, std::unordered_map<std::string, dnd::ChoosableFeature>&& values) {
    choosables[group_name].insert(std::make_move_iterator(values.begin()), std::make_move_iterator(values.end()));
}

bool dnd::Groups::is_group(const std::string& group_name) const {
    return is_string_group(group_name) || is_choosable_group(group_name);
}

bool dnd::Groups::is_string_group(const std::string& group_name) const { return data.contains(group_name); }

bool dnd::Groups::is_choosable_group(const std::string& group_name) const { return choosables.contains(group_name); }

bool dnd::Groups::is_part_of_group(const std::string& name, const std::string& group_name) const {
    if (is_part_of_string_group(name, group_name)) {
        return true;
    }
    return is_part_of_choosable_group(name, group_name);
}

bool dnd::Groups::is_part_of_string_group(const std::string& name, const std::string& string_group_name) const {
    if (!is_string_group(string_group_name)) {
        return false;
    }
    return data.at(string_group_name).contains(name);
}

bool dnd::Groups::is_part_of_choosable_group(const std::string& name, const std::string& choosable_group_name) const {
    if (!is_choosable_group(choosable_group_name)) {
        return false;
    }
    return data.at(choosable_group_name).contains(name);
}

std::string dnd::Groups::status() const {
    std::stringstream sstr;
    sstr << "=== Groups ===\n";
    sstr << "string-based groups parsed: " << data.size() << '\n';
    sstr << "choosable-based groups parsed: " << choosables.size() << '\n';
    return sstr.str();
}
