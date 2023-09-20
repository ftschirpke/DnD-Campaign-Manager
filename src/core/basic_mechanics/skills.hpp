#ifndef SKILLS_HPP_
#define SKILLS_HPP_

#include <dnd_config.hpp>

#include <string>

namespace dnd {

/**
 * @brief Checks whether the given string represents a skill
 * @param skill the string to check
 * @return "true" if string represents a skill, "false" otherwise
 */
bool is_skill(const std::string& skill) noexcept;

} // namespace dnd

#endif // SKILLS_HPP_
