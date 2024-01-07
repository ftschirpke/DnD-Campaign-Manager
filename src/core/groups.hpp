#ifndef GROUPS_HPP_
#define GROUPS_HPP_

#include <dnd_config.hpp>

#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace dnd {

/**
 * @brief A class handling all the groups for a certain session or campaign
 */
class Groups {
public:
    Groups() = default;
    Groups(const Groups&) = delete;
    Groups& operator=(const Groups&) = delete;
    Groups(Groups&&) = default;
    Groups& operator=(Groups&&) = default;

    /**
     * @brief Get the values of a group
     * @param group_name the name of the group
     * @return an unordered set containing the group members (strings)
     * @throws std::out_of_range if group doesn't exist
     */
    std::set<std::string> get_group(const std::string& group_name) const;
    /**
     * @brief Get all group names
     * @return set of all group names
     */
    std::vector<std::string> get_all_group_names() const;
    /**
     * @brief Add a string value to a group (creates the group if it doesn't exist)
     * @param group_name the name of the group
     * @param value the string value to add to the group
     */
    void add(const std::string& group_name, const std::string& value);
    /**
     * @brief Add string values to a group (creates the group if it doesn't exist)
     * @param group_name the name of the group
     * @param values an set of string values to add to the group
     */
    void add(const std::string& group_name, std::set<std::string>&& values);
    /**
     * @brief Set the subgroup of a group (creates the group and subgroup if it doesn't exist)
     * @param group_name the name of the group
     * @param subgroup_name the name of the subgroup
     * @throws std::out_of_range if group doesn't exist
     */
    void set_subgroup(const std::string& group_name, const std::string& subgroup_name);
    /**
     * @brief Set the subgroups of a group (creates the group and subgroups if they don't exist)
     * @param group_name the name of the group
     * @param subgroup_names the names of the subgroups
     * @throws std::out_of_range if group doesn't exist
     */
    void set_subgroups(const std::string& group_name, std::set<std::string>&& subgroup_names);

    bool is_group(const std::string& group_name) const;
    bool is_subgroup(const std::string& subgroup_name, const std::string& group_name) const;
    bool is_member_of_group(const std::string& name, const std::string& group_name) const;
    /**
     * @brief Returns a string describing the amounts of groups parsed
     * @return a string describing the current parsed groups
     */
    std::string status() const;
private:
    // a map containing all the direct members of a group
    std::unordered_map<std::string, std::set<std::string>> members;
    // a map containing all the subgroups of a group
    std::unordered_map<std::string, std::set<std::string>> subgroups;
};

} // namespace dnd

#endif // GROUPS_HPP_
