#ifndef GROUPS_HPP_
#define GROUPS_HPP_

#include <dnd_config.hpp>

#include <string>
#include <unordered_map>
#include <unordered_set>

namespace dnd {

// class Choosable;

/**
 * @brief A class handling all the groups for a certain session or campaign
 * String groups (such as "musical instruments") are mostly cosmetic whereas choosable groups (such as "feats") contain
 * values that can be chosen and affect a characters stats and abilities.
 */
class Groups {
public:
    /**
     * @brief The constructor adds abilities and skills as string groups.
     */
    Groups();
    /**
     * @brief Get the values of a choosable group
     * @param group_name the name of the group
     * @return an unordered map containing the group members (choosables)
     * @throws std::out_of_range if group doesn't exist
     */
    // const std::unordered_map<std::string, Choosable>& get_choosable_group(const std::string& group_name) const;
    /**
     * @brief Get the values of a string group
     * @param group_name the name of the group
     * @return an unordered set containing the group members (strings)
     * @throws std::out_of_range if group doesn't exist
     */
    const std::unordered_set<std::string>& get_string_group(const std::string& group_name) const;
    /**
     * @brief Get all choosable groups
     * @return reference to all the choosable groups saved
     */
    // const std::unordered_map<std::string, std::unordered_map<std::string, Choosable>>& get_all_choosable_groups()
    // const;
    /**
     * @brief Get all string groups
     * @return reference to all the string groups saved
     */
    const std::unordered_map<std::string, std::unordered_set<std::string>>& get_all_string_groups() const;
    /**
     * @brief Add a string value to a group (creates the group if it doesn't exist)
     * @param group_name the name of the group
     * @param value the string value to add to the group
     */
    void add(const std::string& group_name, const std::string& value);
    /**
     * @brief Add string values to a group (creates the group if it doesn't exist)
     * @param group_name the name of the group
     * @param values an unordered set of string values to add to the group
     */
    void add(const std::string& group_name, std::unordered_set<std::string>&& values);
    /**
     * @brief Add a choosable to a group (creates the group if it doesn't exist)
     * and overwrites any existing choosable with the same name
     * @param group_name the name of the group
     * @param value the choosable to add to the group
     */
    // void add(const std::string& group_name, Choosable&& value);
    /**
     * @brief Add choosables to a group (creates the group if it doesn't exist)
     * and overwrites any existing choosables with the same name
     * @param group_name the name of the group
     * @param values an unordered map of choosables to add to the group
     */
    // void add(const std::string& group_name, std::unordered_map<std::string, Choosable>&& values);
    /**
     * @brief Determines wheteher a group with the given name exists.
     * @param group_name the name of the group
     * @return "true" if any group with this name exists, "false" otherwise
     */
    // bool is_group(const std::string& group_name) const;
    /**
     * @brief Determines wheteher a group of strings with given name exists.
     * @param group_name the name of the group
     * @return "true" if a string-group with this name exists, "false" otherwise
     */
    bool is_string_group(const std::string& group_name) const;
    /**
     * @brief Determines wheteher a group of choosables with the given name exists.
     * @param group_name the name of the group
     * @return "true" if a choosable-group with this name exists, "false" otherwise
     */
    // bool is_choosable_group(const std::string& group_name) const;
    /**
     * @brief Determines whether a certain string is part of a given group
     * @param name the string that is supposed to be a part of the group
     * @param group_name the name of the group
     * @return "true" if name is part of the group, "false" otherwise (also when the group does not exist)
     */
    bool is_part_of_group(const std::string& name, const std::string& group_name) const;
    /**
     * @brief Determines whether a certain string is part of a given string group
     * @param name the string that is supposed to be a part of the string group
     * @param group_name the name of the string group
     * @return "true" if name is part of the string group, "false" otherwise (also when the string group does not exist)
     */
    bool is_part_of_string_group(const std::string& name, const std::string& string_group_name) const;
    /**
     * @brief Determines whether a certain string is part of a given choosable group
     * @param name the string that is supposed to be a part of the choosable group
     * @param group_name the name of the choosable group
     * @return "true" if name is part of the choosable group, "false" otherwise (also when the choosable group does not
     * exist)
     */
    // bool is_part_of_choosable_group(const std::string& name, const std::string& choosable_group_name) const;
    /**
     * @brief Returns a string describing the amounts of groups parsed
     * @return a string describing the current parsed groups
     */
    std::string status() const;
private:
    // a map containing all string groups - the members of a string group mapped to the name of the group
    std::unordered_map<std::string, std::unordered_set<std::string>> data;
    // a map containing all choosable groups - the members of a choosable group mapped to the name of the group
    // std::unordered_map<std::string, std::unordered_map<std::string, Choosable>> choosables;
};

} // namespace dnd

#endif // GROUPS_HPP_
