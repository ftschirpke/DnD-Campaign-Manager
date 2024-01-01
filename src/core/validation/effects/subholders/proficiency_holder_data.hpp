#ifndef PROFICIENCY_HOLDER_DATA_HPP_
#define PROFICIENCY_HOLDER_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <set>
#include <string>

#include <core/errors/errors.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class ProficiencyHolderData : public ValidationSubdata {
public:
    ProficiencyHolderData(std::shared_ptr<const ValidationData> parent) noexcept;
    std::strong_ordering operator<=>(const ProficiencyHolderData&) const noexcept = default;
    bool empty() const noexcept;

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

Errors validate_proficiency_holder_for_content(const ProficiencyHolderData& data, const Content& content);

} // namespace dnd

#endif // PROFICIENCY_HOLDER_DATA_HPP_
