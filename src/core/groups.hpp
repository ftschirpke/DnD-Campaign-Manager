#ifndef GROUPS_HPP_
#define GROUPS_HPP_

#include <dnd_config.hpp>

#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace dnd {

class Groups {
public:
    Groups() = default;
    Groups(const Groups&) = delete;
    Groups& operator=(const Groups&) = delete;
    Groups(Groups&&) noexcept = default;
    Groups& operator=(Groups&&) noexcept = default;

    std::set<std::string> get_group(const std::string& group_name) const;
    std::vector<std::string> get_all_group_names() const;
    bool is_group(const std::string& group_name) const;
    bool is_subgroup(const std::string& subgroup_name, const std::string& group_name) const;
    bool is_member_of_group(const std::string& name, const std::string& group_name) const;

    void add(const std::string& group_name, const std::string& value);
    void add(const std::string& group_name, std::set<std::string>&& values);
    void set_subgroup(const std::string& group_name, const std::string& subgroup_name);
    void set_subgroups(const std::string& group_name, std::set<std::string>&& subgroup_names);

    // returns a string describing the amounts of groups parsed
    std::string status() const;
private:
    // a map containing all the direct members of a group
    std::unordered_map<std::string, std::set<std::string>> members;
    // a map containing all the subgroups of a group
    std::unordered_map<std::string, std::set<std::string>> subgroups;
};

} // namespace dnd

#endif // GROUPS_HPP_
