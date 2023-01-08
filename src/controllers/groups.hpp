#ifndef GROUPS_HPP_
#define GROUPS_HPP_

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "models/effects_holder/choosable.hpp"

namespace dnd {

class Groups {
public:
    const std::unordered_set<std::string>& get(const std::string& group_name) const;
    // add an empty group
    void add(const std::string& group_name);
    // add a value to a group (creates the group if it doesn't exist)
    void add(const std::string& group_name, const std::string& value);
    // add values to a group (creates the group if it doesn't exist)
    void add(const std::string& group_name, std::unordered_set<std::string>&& values);
    // add a choosable to a group (creates the group if it doesn't exist)
    void add(const std::string& group_name, Choosable&& value);
    // add choosables to a group (creates the group if it doesn't exist)
    void add(const std::string& group_name, std::unordered_map<std::string, Choosable>&& values);
    void printStatus() const;
private:
    std::unordered_map<std::string, std::unordered_set<std::string>> data;
    std::unordered_map<std::string, std::unordered_map<std::string, Choosable>> choosables;
};

inline const std::unordered_set<std::string>& Groups::get(const std::string& group_name) const {
    return data.at(group_name);
}

inline void Groups::add(const std::string& group_name) { data[group_name]; }

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

} // namespace dnd

#endif // GROUPS_HPP_
