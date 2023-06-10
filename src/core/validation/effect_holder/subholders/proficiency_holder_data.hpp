#ifndef PROFICIENCY_HOLDER_DATA_HPP_
#define PROFICIENCY_HOLDER_DATA_HPP_

#include <dnd_config.hpp>

#include <set>
#include <string>

#include <core/errors/errors.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class ProficiencyHolderData : public ValidationSubdata {
public:
    ProficiencyHolderData(const ValidationData* parent) noexcept;
    /**
     * @brief Validates the data
     * @return the errors that occured during validation
     */
    virtual Errors validate() const override;
    /**
     * @brief Validates the relations of the data to other content pieces with regard to a given content holder
     * @param content the content holder to validate the relations against
     * @return the errors that occured during validation
     */
    virtual Errors validate_relations(const ContentHolder* content) const override;

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

} // namespace dnd

#endif // PROFICIENCY_HOLDER_DATA_HPP_
