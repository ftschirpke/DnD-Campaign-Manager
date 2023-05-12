#ifndef GROUPS_HPP_
#define GROUPS_HPP_

#include "dnd_config.hpp"

#include <iterator>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "models/effect_holder/choosable.hpp"

namespace dnd {

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
    const std::unordered_map<std::string, Choosable>& getChoosableGroup(const std::string& group_name) const;
    /**
     * @brief Get the values of a string group
     * @param group_name the name of the group
     * @return an unordered set containing the group members (strings)
     * @throws std::out_of_range if group doesn't exist
     */
    const std::unordered_set<std::string>& getStringGroup(const std::string& group_name) const;
    /**
     * @brief Get all choosable groups
     * @return reference to all the choosable groups saved
     */
    const std::unordered_map<std::string, std::unordered_map<std::string, Choosable>>& getAllChoosableGroups() const;
    /**
     * @brief Get all string groups
     * @return reference to all the string groups saved
     */
    const std::unordered_map<std::string, std::unordered_set<std::string>>& getAllStringGroups() const;
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
    void add(const std::string& group_name, Choosable&& value);
    /**
     * @brief Add choosables to a group (creates the group if it doesn't exist)
     * and overwrites any existing choosables with the same name
     * @param group_name the name of the group
     * @param values an unordered map of choosables to add to the group
     */
    void add(const std::string& group_name, std::unordered_map<std::string, Choosable>&& values);
    /**
     * @brief Determines wheteher a group with the given name exists.
     * @param group_name the name of the group
     * @return "true" if any group with this name exists, "false" otherwise
     */
    bool isGroup(const std::string& group_name) const;
    /**
     * @brief Determines wheteher a group of strings with given name exists.
     * @param group_name the name of the group
     * @return "true" if a string-group with this name exists, "false" otherwise
     */
    bool isStringGroup(const std::string& group_name) const;
    /**
     * @brief Determines wheteher a group of choosables with the given name exists.
     * @param group_name the name of the group
     * @return "true" if a choosable-group with this name exists, "false" otherwise
     */
    bool isChoosableGroup(const std::string& group_name) const;
    /**
     * @brief Determines whether a certain string is part of a given group.
     * @param name the string that is supposed to be a part of the group
     * @param group_name the name of the group
     * @return "true" if name is part of the group, "false" otherwise (also when the group does not exist)
     */
    bool isPartOfGroup(const std::string& name, const std::string& group_name) const;
    /**
     * @brief Returns a string describing the amounts of groups parsed
     * @return a string describing the current parsed groups
     */
    std::string printStatus() const;
private:
    // a map containing all string groups - the members of a string group mapped to the name of the group
    std::unordered_map<std::string, std::unordered_set<std::string>> data;
    // a map containing all choosable groups - the members of a choosable group mapped to the name of the group
    std::unordered_map<std::string, std::unordered_map<std::string, Choosable>> choosables;
};

inline const std::unordered_map<std::string, Choosable>& Groups::getChoosableGroup(const std::string& group_name
) const {
    return choosables.at(group_name);
}

inline const std::unordered_set<std::string>& Groups::getStringGroup(const std::string& group_name) const {
    return data.at(group_name);
}

inline const std::unordered_map<std::string, std::unordered_map<std::string, Choosable>>& Groups::getAllChoosableGroups(
) const {
    return choosables;
}

inline const std::unordered_map<std::string, std::unordered_set<std::string>>& Groups::getAllStringGroups() const {
    return data;
}

inline void Groups::add(const std::string& group_name, const std::string& value) { data[group_name].insert(value); }

inline void Groups::add(const std::string& group_name, std::unordered_set<std::string>&& values) {
    data[group_name].insert(std::make_move_iterator(values.begin()), std::make_move_iterator(values.end()));
}

inline void Groups::add(const std::string& group_name, Choosable&& value) {
    choosables[group_name].emplace(value.name, std::move(value));
}

inline void Groups::add(const std::string& group_name, std::unordered_map<std::string, Choosable>&& values) {
    choosables[group_name].insert(std::make_move_iterator(values.begin()), std::make_move_iterator(values.end()));
}

inline bool Groups::isGroup(const std::string& group_name) const {
    return isStringGroup(group_name) || isChoosableGroup(group_name);
}

} // namespace dnd

#endif // GROUPS_HPP_
