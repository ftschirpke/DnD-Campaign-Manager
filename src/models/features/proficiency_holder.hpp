#ifndef PROFICIENCY_HOLDER_HPP_
#define PROFICIENCY_HOLDER_HPP_

#include <string>
#include <unordered_set>

namespace dnd {

// This class holds a creature's proficiencies, its known languages, and special senses it has.
class ProficiencyHolder {
public:
    std::unordered_set<std::string> armor, weapons, tools, skills, saving_throws, languages, senses;
};

} // namespace dnd

#endif // PROFICIENCY_HOLDER_HPP_
