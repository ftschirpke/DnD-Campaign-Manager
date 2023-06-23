#ifndef LITERAL_EFFECT_HPP_
#define LITERAL_EFFECT_HPP_

#include <dnd_config.hpp>

#include <string>
#include <string_view>
#include <unordered_map>

#include <core/models/effect_holder/effect/effect.hpp>

namespace dnd {

/**
 * @brief A class that represents an effect that modifies a character value by a literal (boolean, integer, or float)
 */
class LiteralEffect : public Effect {
public:
    /**
     * @brief Constructs an Effect with the attribute it affects, its execution time, and the name and boolean value for
     * the operation.
     * CAREFUL: if the operation is not found, the construction doesn't fail, but this "effect" will do nothing.
     * @param affected_attribute the name of the attribute whose calculation is affected by this effect
     * @param time the time at which this effect should be applied in the order of execution
     * @param operation_name the name of the mathematical operation that is performed when applying this effect
     * @param value the boolean value that is used in the operation
     */
    LiteralEffect(
        const std::string& affected_attribute, EffectTime time, const std::string& operation_name, bool value
    ) noexcept;
    /**
     * @brief Constructs an Effect with the attribute it affects, its execution time, and the name and boolean value for
     * the operation.
     * CAREFUL: if the operation is not found, the construction doesn't fail, but this "effect" will do nothing.
     * @param affected_attribute the name of the attribute whose calculation is affected by this effect
     * @param time the time at which this effect should be applied in the order of execution
     * @param operation_name the name of the mathematical operation that is performed when applying this effect
     * @param value the boolean value that is used in the operation
     */
    LiteralEffect(
        std::string_view affected_attribute, EffectTime time, std::string_view operation_name, bool value
    ) noexcept;
    /**
     * @brief Constructs an Effect with the attribute it affects, its execution time, and the name and integer value for
     * the operation.
     * CAREFUL: if the operation is not found, the construction doesn't fail, but this "effect" will do nothing.
     * @param affected_attribute the name of the attribute whose calculation is affected by this effect
     * @param time the time at which this effect should be applied in the order of execution
     * @param operation_name the name of the mathematical operation that is performed when applying this effect
     * @param value the integer value that is used in the operation
     */
    LiteralEffect(
        const std::string& affected_attribute, EffectTime time, const std::string& operation_name, int value
    ) noexcept;
    /**
     * @brief Constructs an Effect with the attribute it affects, its execution time, and the name and integer value for
     * the operation.
     * CAREFUL: if the operation is not found, the construction doesn't fail, but this "effect" will do nothing.
     * @param affected_attribute the name of the attribute whose calculation is affected by this effect
     * @param time the time at which this effect should be applied in the order of execution
     * @param operation_name the name of the mathematical operation that is performed when applying this effect
     * @param value the integer value that is used in the operation
     */
    LiteralEffect(
        std::string_view affected_attribute, EffectTime time, std::string_view operation_name, int value
    ) noexcept;
    /**
     * @brief Constructs an Effect with the attribute it affects, its execution time, and the name and float value for
     * the operation.
     * CAREFUL: if the operation is not found, the construction doesn't fail, but this "effect" will do nothing.
     * @param affected_attribute the name of the attribute whose calculation is affected by this effect
     * @param time the time at which this effect should be applied in the order of execution
     * @param operation_name the name of the mathematical operation that is performed when applying this effect
     * @param value the float value that is used in the operation
     */
    LiteralEffect(
        const std::string& affected_attribute, EffectTime time, const std::string& operation_name, float value
    ) noexcept;
    /**
     * @brief Constructs an Effect with the attribute it affects, its execution time, and the name and float value for
     * the operation.
     * CAREFUL: if the operation is not found, the construction doesn't fail, but this "effect" will do nothing.
     * @param affected_attribute the name of the attribute whose calculation is affected by this effect
     * @param time the time at which this effect should be applied in the order of execution
     * @param operation_name the name of the mathematical operation that is performed when applying this effect
     * @param value the float value that is used in the operation
     */
    LiteralEffect(
        std::string_view affected_attribute, EffectTime time, std::string_view operation_name, float value
    ) noexcept;
    /**
     * @brief Applies the effect to a character's attributes given the attributes and constants of the character
     * @param attributes character attributes may be used for calculation and one of them will be changed
     * @param constants character constants may be used for calculation
     */
    virtual void apply_to(
        std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const override final;
private:
    int value;
};

} // namespace dnd

#endif // LITERAL_EFFECT_HPP_
