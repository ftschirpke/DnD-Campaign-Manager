#ifndef PROFICIENCY_HOLDER_HPP_
#define PROFICIENCY_HOLDER_HPP_

#include "dnd_config.hpp"

#include <string>
#include <unordered_set>

namespace dnd {

/**
 * @brief This class holds a characters's proficiencies, its known languages, and special senses it has.
 */
class ProficiencyHolder {
public:
    // the types of armor the character is proficient with
    std::unordered_set<std::string> armor;
    // the types of weapons the character is proficient with
    std::unordered_set<std::string> weapons;
    // the tools the character is proficient with
    std::unordered_set<std::string> tools;
    // the skills the character is proficient at
    std::unordered_set<std::string> skills;
    // the saving throws the character is proficient at
    std::unordered_set<std::string> saving_throws;
    // the languages the character knows
    std::unordered_set<std::string> languages;
    // special types of senses the character has e.g. darkvision
    std::unordered_set<std::string> senses;
    /**
     * @brief Returns true if the ProficiencyHolder is empty
     * @return "true" if the ProficiencyHolder is empty, "false" otherwise
     */
    bool empty() const;
};

inline bool ProficiencyHolder::empty() const {
    return armor.empty() && weapons.empty() && tools.empty() && skills.empty() && saving_throws.empty()
           && languages.empty() && senses.empty();
}

} // namespace dnd

#endif // PROFICIENCY_HOLDER_HPP_
