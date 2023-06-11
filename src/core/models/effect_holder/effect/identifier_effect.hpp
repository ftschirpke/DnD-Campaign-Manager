#ifndef IDENTIFIER_EFFECT_HPP_
#define IDENTIFIER_EFFECT_HPP_

#include <dnd_config.hpp>

#include <string>
#include <string_view>
#include <unordered_map>

#include <core/models/effect_holder/effect/effect.hpp>

namespace dnd {

class IdentifierEffect : public Effect {
public:
    /**
     * @brief Constructs an Effect with the attribute it affects, its execution time, and the name and value identifier
     * for the operation.
     * CAREFUL: if the operation is not found, the construction doesn't fail, but this "effect" will do nothing.
     * @param affected_attribute the name of the attribute whose calculation is affected by this effect
     * @param time the time at which this effect should be applied in the order of execution
     * @param operation_name the name of the mathematical operation that is performed when applying this effect
     * @param value_identifier the identifier for the value that is used in the operation
     */
    IdentifierEffect(
        const std::string& affected_attribute, EffectTime time, const std::string& operation_name,
        const std::string& value_identifier
    ) noexcept;
    /**
     * @brief Constructs an Effect with the attribute it affects, its execution time, and the name and value identifier
     * for the operation.
     * CAREFUL: if the operation is not found, the construction doesn't fail, but this "effect" will do nothing.
     * @param affected_attribute the name of the attribute whose calculation is affected by this effect
     * @param time the time at which this effect should be applied in the order of execution
     * @param operation_name the name of the mathematical operation that is performed when applying this effect
     * @param value_identifier the identifier for the value that is used in the operation
     */
    IdentifierEffect(
        std::string_view affected_attribute, EffectTime time, std::string_view operation_name,
        std::string_view value_identifier
    ) noexcept;
    /**
     * @brief Applies the effect to a character's attributes given the attributes and constants of the character
     * @param attributes character attributes may be used for calculation and one of them will be changed
     * @param constants character constants may be used for calculation
     * @throws std::out_of_range if no value is found for the value identifier
     */
    virtual void apply_to(
        std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const override final;
private:
    std::string value_identifier;
};

} // namespace dnd

#endif // IDENTIFIER_EFFECT_HPP_
