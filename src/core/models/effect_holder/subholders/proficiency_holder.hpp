#ifndef PROFICIENCY_HOLDER_HPP_
#define PROFICIENCY_HOLDER_HPP_

#include <dnd_config.hpp>

#include <string>
#include <vector>

#include <core/controllers/content_holder.hpp>
#include <core/validation/effect_holder/subholders/proficiency_holder_data.hpp>

namespace dnd {

class ProficiencyHolder {
public:
    /**
     * @brief Constructs a ProficiencyHolder from the given data and content
     * @param data the data to construct the ProficiencyHolder from
     * @param content the content to use for the proficiencies
     * @return the constructed ProficiencyHolder
     * @throws dnd::invalid_data if the given data is invalid or is incompatible with the given content
     */
    static ProficiencyHolder create(ProficiencyHolderData&& data, const ContentHolder* content);

    const std::vector<std::string>& get_armor() const noexcept;
    const std::vector<std::string>& get_weapons() const noexcept;
    const std::vector<std::string>& get_tools() const noexcept;
    const std::vector<std::string>& get_skills() const noexcept;
    const std::vector<std::string>& get_saving_throws() const noexcept;
    const std::vector<std::string>& get_languages() const noexcept;
    const std::vector<std::string>& get_senses() const noexcept;
private:
    ProficiencyHolder(
        std::vector<std::string>&& armor, std::vector<std::string>&& weapons, std::vector<std::string>&& tools,
        std::vector<std::string>&& skills, std::vector<std::string>&& saving_throws,
        std::vector<std::string>&& languages, std::vector<std::string>&& senses
    ) noexcept;

    std::vector<std::string> armor;
    std::vector<std::string> weapons;
    std::vector<std::string> tools;
    std::vector<std::string> skills;
    std::vector<std::string> saving_throws;
    std::vector<std::string> languages;
    std::vector<std::string> senses;
};

} // namespace dnd

#endif // PROFICIENCY_HOLDER_HPP_
