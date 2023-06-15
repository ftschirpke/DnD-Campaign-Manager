#ifndef DECISION_DATA_HPP_
#define DECISION_DATA_HPP_

#include <dnd_config.hpp>

#include <string>

#include <core/errors/errors.hpp>
#include <core/validation/effect_holder/effect_holder_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class CharacterData;

class DecisionData : public ValidationSubdata {
public:
    DecisionData(const CharacterData* parent) noexcept;
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

    std::string choice_name;
    EffectHolderData effects_data;
};

} // namespace dnd

#endif // DECISION_DATA_HPP_
