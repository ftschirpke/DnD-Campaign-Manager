#ifndef PROFICIENCY_HOLDER_HPP_
#define PROFICIENCY_HOLDER_HPP_

#include <dnd_config.hpp>

#include <string>
#include <vector>

#include <core/validation/effect_holder/subholders/proficiency_holder_data.hpp>

namespace dnd {

class ContentHolder;

/**
 * @brief This class holds a characters's proficiencies, its known languages, and special senses it has.
 */
class ProficiencyHolder {
public:
    /**
     * @brief Constructs a ProficiencyHolder from the given data and content
     * @param data the data to construct the ProficiencyHolder from
     * @param content the content to use for the proficiencies
     * @return the constructed ProficiencyHolder
     * @throws dnd::invalid_data if the given data is invalid or is incompatible with the given content
     */
    static ProficiencyHolder create(ProficiencyHolderData&& data, const ContentHolder& content);

    /**
     * @brief Returns whether the ProficiencyHolder is empty
     * @return "true" if the ProficiencyHolder is empty, "false" otherwise
     */
    bool empty() const;

    /**
     * @brief Returns the types of armor the character is proficient with
     * @return the types of armor the character is proficient with
     */
    const std::vector<std::string>& get_armor() const noexcept;
    /**
     * @brief Returns the types of weapons the character is proficient with
     * @return the types of weapons the character is proficient with
     */
    const std::vector<std::string>& get_weapons() const noexcept;
    /**
     * @brief Returns the tools the character is proficient with
     * @return the tools the character is proficient with
     */
    const std::vector<std::string>& get_tools() const noexcept;
    /**
     * @brief Returns the skills the character is proficient at
     * @return the skills the character is proficient at
     */
    const std::vector<std::string>& get_skills() const noexcept;
    /**
     * @brief Returns the saving throws the character is proficient at
     * @return the saving throws the character is proficient at
     */
    const std::vector<std::string>& get_saving_throws() const noexcept;
    /**
     * @brief Returns the languages the character knows
     * @return the languages the character knows
     */
    const std::vector<std::string>& get_languages() const noexcept;
    /**
     * @brief Return the special types of senses the character has e.g. darkvision
     * @return the special types of senses the character has e.g. darkvision
     */
    const std::vector<std::string>& get_senses() const noexcept;
private:
    ProficiencyHolder(
        std::vector<std::string>&& armor, std::vector<std::string>&& weapons, std::vector<std::string>&& tools,
        std::vector<std::string>&& skills, std::vector<std::string>&& saving_throws,
        std::vector<std::string>&& languages, std::vector<std::string>&& senses
    ) noexcept;

    // the types of armor the character is proficient with
    std::vector<std::string> armor;
    // the types of weapons the character is proficient with
    std::vector<std::string> weapons;
    // the tools the character is proficient with
    std::vector<std::string> tools;
    // the skills the character is proficient at
    std::vector<std::string> skills;
    // the saving throws the character is proficient at
    std::vector<std::string> saving_throws;
    // the languages the character knows
    std::vector<std::string> languages;
    // special types of senses the character has e.g. darkvision
    std::vector<std::string> senses;
};

} // namespace dnd

#endif // PROFICIENCY_HOLDER_HPP_
