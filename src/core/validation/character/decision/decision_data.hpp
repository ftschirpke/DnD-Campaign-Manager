#ifndef DECISION_DATA_HPP_
#define DECISION_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <map>
#include <string>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class CharacterData;
class EffectHolder;

class DecisionData : public ValidationSubdata {
public:
    DecisionData(const CharacterData* parent, const EffectHolder* target) noexcept;
    std::strong_ordering operator<=>(const DecisionData&) const noexcept = default;
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

    const CharacterData* get_character_data() const noexcept;
    const EffectHolder* get_target() const noexcept;

    void set_target(const EffectHolder* new_target) noexcept;

    std::string feature_name;
    std::map<std::string, std::vector<std::string>> selections;
private:
    const CharacterData* character_data;
    const EffectHolder* target;
};

} // namespace dnd

#endif // DECISION_DATA_HPP_
