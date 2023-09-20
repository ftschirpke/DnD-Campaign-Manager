#ifndef SKILLS_HPP_
#define SKILLS_HPP_

#include <dnd_config.hpp>

#include <map>
#include <string>
#include <vector>

namespace dnd {

/**
 * @brief Checks whether the given string represents a skill
 * @param skill the string to check
 * @return "true" if string represents a skill, "false" otherwise
 */
bool is_skill(const std::string& skill) noexcept;

/**
 * @brief Returns a vector of all skills
 * @return a vector of all skills
 */
std::vector<std::string> get_skills() noexcept;

/**
 * @brief Returns all skills and their corresponding abilities as a map
 * @return the map of all skills and their corresponding abilities
 */
std::map<std::string, std::string> get_skill_ability_map() noexcept;


} // namespace dnd

#endif // SKILLS_HPP_
