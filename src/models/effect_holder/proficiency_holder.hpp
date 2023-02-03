#ifndef PROFICIENCY_HOLDER_HPP_
#define PROFICIENCY_HOLDER_HPP_

#include "dnd_config.hpp"

#include <string>
#include <unordered_set>

namespace dnd {

// This class holds a creature's proficiencies, its known languages, and special senses it has.
class ProficiencyHolder {
public:
    std::unordered_set<std::string> armor, weapons, tools, skills, saving_throws, languages, senses;
    bool empty() const;
};

inline bool ProficiencyHolder::empty() const {
    return armor.empty() && weapons.empty() && tools.empty() && skills.empty() && saving_throws.empty()
           && languages.empty() && senses.empty();
}

} // namespace dnd

#endif // PROFICIENCY_HOLDER_HPP_
