#include <dnd_config.hpp>

#include "groups.hpp"

#include <iterator>
#include <set>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <fmt/format.h>

#include <core/basic_mechanics/abilities.hpp>
#include <core/basic_mechanics/skills.hpp>
#include <core/utils/string_manipulation.hpp>

dnd::Groups::Groups() {
    members["abilities"] = std::set<std::string>(ability_cstrings_inorder.begin(), ability_cstrings_inorder.end());

    std::string lowercase_skill;
    for (const auto& [skill, _] : skill_abilities) {
        lowercase_skill = string_lowercase_copy(skill);

        size_t idx = lowercase_skill.find('_');
        while (idx != std::string::npos) {
            lowercase_skill[idx] = ' ';
            idx = lowercase_skill.find('_');
        }
        members["skills"].insert(lowercase_skill);
    }
}

std::set<std::string> dnd::Groups::get_group(const std::string& group_name) const {
    if (!is_group(group_name)) {
        return {};
    }
    std::set<std::string> group_members = members.at(group_name);
    for (const auto& subgroup_name : subgroups.at(group_name)) {
        std::set<std::string> subgroup_members = get_group(subgroup_name);
        group_members.insert(
            std::make_move_iterator(subgroup_members.begin()), std::make_move_iterator(subgroup_members.end())
        );
    }
    return group_members;
}

std::vector<std::string> dnd::Groups::get_all_group_names() const {
    std::vector<std::string> group_names;
    group_names.reserve(members.size());
    for (const auto& [group_name, _] : members) {
        group_names.push_back(group_name);
    }
    return group_names;
}

void dnd::Groups::add(const std::string& group_name, const std::string& value) { members[group_name].insert(value); }

void dnd::Groups::add(const std::string& group_name, std::set<std::string>&& values) {
    members[group_name].insert(std::make_move_iterator(values.begin()), std::make_move_iterator(values.end()));
}

void dnd::Groups::set_subgroup(const std::string& group_name, const std::string& subgroup_name) {
    subgroups[group_name].insert(subgroup_name);
    members[group_name];
    members[subgroup_name];
}

void dnd::Groups::set_subgroups(const std::string& group_name, std::set<std::string>&& subgroup_names) {
    subgroups[group_name].insert(
        std::make_move_iterator(subgroup_names.begin()), std::make_move_iterator(subgroup_names.end())
    );
    members[group_name];
    for (const auto& subgroup_name : subgroup_names) {
        members[subgroup_name];
    }
}

bool dnd::Groups::is_group(const std::string& group_name) const { return members.contains(group_name); }

bool dnd::Groups::is_subgroup(const std::string& subgroup_name, const std::string& group_name) const {
    if (!subgroups.contains(group_name)) {
        return false;
    }
    return subgroups.at(group_name).contains(subgroup_name);
}

bool dnd::Groups::is_part_of_group(const std::string& name, const std::string& group_name) const {
    if (members.at(group_name).contains(name)) {
        return true;
    }
    if (!subgroups.contains(group_name)) {
        return false;
    }
    for (const auto& subgroup_name : subgroups.at(group_name)) {
        if (is_part_of_group(name, subgroup_name)) {
            return true;
        }
    }
    return false;
}

std::string dnd::Groups::status() const { return fmt::format("=== Groups ===\ngroups parsed: {}\n", members.size()); }
