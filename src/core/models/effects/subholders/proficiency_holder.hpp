#ifndef PROFICIENCY_HOLDER_HPP_
#define PROFICIENCY_HOLDER_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <set>
#include <string>

#include <vector>

#include <core/data_result.hpp>

namespace dnd {

class Content;

/**
 * @brief This class holds a characters's proficiencies, its known languages, and special senses it has.
 */
class ProficiencyHolder {
public:
    struct Data;

    static CreateResult<ProficiencyHolder> create_for(Data&& data, const Content& content);

    const std::vector<std::string>& get_armor_proficiencies() const;
    const std::vector<std::string>& get_weapon_proficiencies() const;
    const std::vector<std::string>& get_tool_proficiencies() const;
    const std::vector<std::string>& get_skill_proficiencies() const;
    const std::vector<std::string>& get_saving_throw_proficiencies() const;
    const std::vector<std::string>& get_known_languages() const;
    const std::vector<std::string>& get_senses() const;

    bool empty() const;
    void merge(ProficiencyHolder&& other);
private:
    ProficiencyHolder(
        std::vector<std::string>&& armor, std::vector<std::string>&& weapons, std::vector<std::string>&& tools,
        std::vector<std::string>&& skills, std::vector<std::string>&& saving_throws,
        std::vector<std::string>&& languages, std::vector<std::string>&& senses
    );

    std::vector<std::string> armor_proficiencies;
    std::vector<std::string> weapon_proficiencies;
    std::vector<std::string> tool_proficiencies;
    std::vector<std::string> skill_proficiencies;
    std::vector<std::string> saving_throw_proficiencies;
    std::vector<std::string> known_languages;
    std::vector<std::string> senses;
};

struct ProficiencyHolder::Data {
    std::strong_ordering operator<=>(const ProficiencyHolder::Data&) const = default;
    bool empty() const;

    // the types of armor the character is proficient with
    std::set<std::string> armor;
    // the types of weapons the character is proficient with
    std::set<std::string> weapons;
    // the tools the character is proficient with
    std::set<std::string> tools;
    // the skills the character is proficient at
    std::set<std::string> skills;
    // the saving throws the character is proficient at
    std::set<std::string> saving_throws;
    // the languages the character knows
    std::set<std::string> languages;
    // special types of senses the character has e.g. darkvision
    std::set<std::string> senses;
};

inline bool ProficiencyHolder::Data::empty() const {
    return armor.empty() && weapons.empty() && tools.empty() && skills.empty() && saving_throws.empty()
           && languages.empty() && senses.empty();
}

} // namespace dnd

#endif // PROFICIENCY_HOLDER_HPP_
