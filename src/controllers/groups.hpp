#ifndef GROUPS_HPP_
#define GROUPS_HPP_

#include <string>
#include <unordered_map>
#include <unordered_set>

namespace dnd {

class Groups {
public:
    const std::unordered_set<std::string>& get(const std::string& group_name) const;
    // add an empty group
    void addGroup(const std::string& group_name);
    // add a group and its values
    void addGroup(const std::string& group_name, const std::unordered_set<std::string>& values);
    void addGroup(const std::string& group_name, std::unordered_set<std::string>&& values);
    void addGroup(std::string&& group_name, std::unordered_set<std::string>&& values);
    // add a value to a group (creates the group if it doesn't exist)
    void addValueToGroup(const std::string& value, const std::string& group_name);
    // add a value to an existing group (throws exception if group doesn't exist)
    void addValueToExistingGroup(const std::string& value, const std::string& group_name);
private:
    std::unordered_map<std::string, std::unordered_set<std::string>> data;
};

inline const std::unordered_set<std::string>& Groups::get(const std::string& group_name) const {
    return data.at(group_name);
}

inline void Groups::addGroup(const std::string& group_name) { data[group_name]; }

inline void Groups::addGroup(const std::string& group_name, const std::unordered_set<std::string>& values) {
    data.emplace(group_name, values);
}

inline void Groups::addGroup(const std::string& group_name, std::unordered_set<std::string>&& values) {
    data.emplace(group_name, values);
}

inline void Groups::addGroup(std::string&& group_name, std::unordered_set<std::string>&& values) {
    data.emplace(group_name, values);
}

inline void Groups::addValueToGroup(const std::string& value, const std::string& group_name) {
    data[group_name].insert(value);
}

inline void Groups::addValueToExistingGroup(const std::string& value, const std::string& group_name) {
    data.at(group_name).insert(value);
}

} // namespace dnd

#endif // GROUPS_HPP_
