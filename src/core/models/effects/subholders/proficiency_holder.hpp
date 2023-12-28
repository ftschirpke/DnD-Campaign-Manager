#ifndef PROFICIENCY_HOLDER_HPP_
#define PROFICIENCY_HOLDER_HPP_

#include <dnd_config.hpp>

#include <string>
#include <vector>

#include <core/validation/effects/subholders/proficiency_holder_data.hpp>

namespace dnd {

class Content;

/**
 * @brief This class holds a characters's proficiencies, its known languages, and special senses it has.
 */
class ProficiencyHolder {
public:
    using Data = ProficiencyHolderData;

    /**
     * @brief Constructs a ProficiencyHolder from the given data and content
     * @param data the data to construct the ProficiencyHolder from
     * @param content the content to use for the proficiencies
     * @return the constructed ProficiencyHolder
     * @throws invalid_data if the given data is invalid or is incompatible with the given content
     */
    static ProficiencyHolder create(Data&& data, const Content& content);

    const std::vector<std::string>& get_armor_proficiencies() const noexcept;
    const std::vector<std::string>& get_weapon_proficiencies() const noexcept;
    const std::vector<std::string>& get_tool_proficiencies() const noexcept;
    const std::vector<std::string>& get_skill_proficiencies() const noexcept;
    const std::vector<std::string>& get_saving_throw_proficiencies() const noexcept;
    const std::vector<std::string>& get_known_languages() const noexcept;
    const std::vector<std::string>& get_senses() const noexcept;

    bool empty() const;
    void merge(ProficiencyHolder&& other);
private:
    ProficiencyHolder(
        std::vector<std::string>&& armor, std::vector<std::string>&& weapons, std::vector<std::string>&& tools,
        std::vector<std::string>&& skills, std::vector<std::string>&& saving_throws,
        std::vector<std::string>&& languages, std::vector<std::string>&& senses
    ) noexcept;

    std::vector<std::string> armor_proficiencies;
    std::vector<std::string> weapon_proficiencies;
    std::vector<std::string> tool_proficiencies;
    std::vector<std::string> skill_proficiencies;
    std::vector<std::string> saving_throw_proficiencies;
    std::vector<std::string> known_languages;
    std::vector<std::string> senses;
};

} // namespace dnd

#endif // PROFICIENCY_HOLDER_HPP_
