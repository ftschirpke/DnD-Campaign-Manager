#ifndef CHARACTER_BASIS_DATA_HPP_
#define CHARACTER_BASIS_DATA_HPP_

#include <dnd_config.hpp>

#include <string>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class ContentHolder;

class CharacterBasisData : public ValidationSubdata {
public:
    CharacterBasisData(const ValidationData* parent) noexcept;
    int operator<=>(const CharacterBasisData&) const noexcept = default;
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
    virtual Errors validate_relations(const ContentHolder& content) const override;

    std::string race_name;
    std::string subrace_name;
    std::string class_name;
    std::string subclass_name;
};

} // namespace dnd

#endif // CHARACTER_BASIS_DATA_HPP_
