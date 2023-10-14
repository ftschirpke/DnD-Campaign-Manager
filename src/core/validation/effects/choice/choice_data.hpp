#ifndef CHOICE_DATA_HPP_
#define CHOICE_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <string>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

enum class ChoiceType;

class ChoiceData : public ValidationSubdata {
public:
    ChoiceData(const ValidationData* parent) noexcept;
    std::strong_ordering operator<=>(const ChoiceData&) const noexcept = default;
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
    virtual Errors validate_relations(const Content& content) const override;

    /**
     * @brief Determines the type of the choice
     * @return the type of the choice
     */
    ChoiceType determine_type() const;

    std::string attribute_name;
    int amount;
    std::vector<std::string> group_names;
    std::vector<std::string> explicit_choices;
};

} // namespace dnd

#endif // CHOICE_DATA_HPP_