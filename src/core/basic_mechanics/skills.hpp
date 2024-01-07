#ifndef SKILLS_HPP_
#define SKILLS_HPP_

#include <dnd_config.hpp>

#include <map>
#include <string>
#include <vector>

namespace dnd {

bool is_skill(const std::string& skill) noexcept;

std::vector<std::string> get_all_skills() noexcept;

std::map<std::string, std::string> get_abilities_for_all_skills() noexcept;

} // namespace dnd

#endif // SKILLS_HPP_
