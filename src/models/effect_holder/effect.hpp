#ifndef EFFECT_HPP_
#define EFFECT_HPP_

#include "dnd_config.hpp"

#include <algorithm>
#include <array>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace dnd {

/**
 * @brief An enum to manage the calculation of effects that might rely on the proper order of execution
 */
enum EffectTime {
    // earliest - executed first, e.g. setting a new default value
    EARLIEST,
    // early - executed early, e.g. multiplication that shouldn't affect added boni
    EARLY,
    // normal - intended for normal boni e.g. -1, +1, +2 etc.
    NORMAL,
    // late - executed late, e.g. multiplication that should affect added boni
    LATE,
    // latest - executed latest, e.g. setting fixed value
    LATEST
};

// the effect times in their proper order paired with their names
const std::array<std::pair<const char*, EffectTime>, 5> effect_times_in_order = {
    std::pair("earliest", EffectTime::EARLIEST), std::pair("early", EffectTime::EARLY),
    std::pair("normal", EffectTime::NORMAL),     std::pair("late", EffectTime::LATE),
    std::pair("latest", EffectTime::LATEST),
};

/**
 * @brief A class representing the effect a feature has on the calculation of an attribute
 */
class Effect {
public:
    /**
     * @brief Constructs an Effect with the attribute it affects, the name of the operation, and its execution time.
     * @param affected_attribute the name of the attribute whose calculation is affected by this effect
     * @param op_name the string representation of the operation that is performed when applying this effect
     * @param time the time at which this effect should be applied in the order of execution
     */
    Effect(const std::string& affected_attribute, const std::string& op_name, const EffectTime time) noexcept;
    virtual ~Effect() noexcept = default;
    /**
     * @brief Applies the effect to a character's attributes given the attributes and constants of the character
     * @param attributes character attributes may be used for calculation and one of them will be changed
     * @param constants character constants may be used for calculation
     */
    virtual void applyTo(
        std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const = 0;

    // the effect calculations supported for integers paired with their c-style string representations
    static const std::array<std::pair<const char*, int (*)(int, int)>, 6> int_operators;
    // the effect calculations supported for floats paired with their c-style string representations
    static const std::array<std::pair<const char*, int (*)(int, float)>, 2> float_operators;
    // the name of the attribute whose calculation is affected by this effect
    const std::string affected_attribute;
    // the string representation of the operation that is performed when applying this effect
    const std::string op_name;
    // the time at which this effect should be applied in the order of execution
    const EffectTime time;
};

/**
 * @brief A class representing an effect that changes an attribute using a fixed integer value
 */
class IntNumEffect : public Effect {
public:
    /**
     * @brief Constructs an IntNumEffect with the attribute it affects, the name of the operation, its execution time,
     * and the integer used for calculation.
     * @param affected_attribute the name of the attribute whose calculation is affected by this effect
     * @param op_name the string representation of the operation that is performed when applying this effect
     * @param time the time at which this effect should be applied in the order of execution
     * @param value the fixed integer value used in the operation
     */
    IntNumEffect(const std::string& affected_attribute, const std::string& op_name, const EffectTime time, int value);
    /**
     * @brief Applies the integer effect to a character's attributes given the attributes and constants of the character
     * @param attributes character attributes may be used for calculation and one of them will be changed
     * @param constants character constants may be used for calculation
     */
    virtual void applyTo(
        std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const override;

    // the fixed integer value used in the operation
    const int value;
protected:
    // the operation to be applied to the attribute using the fixed integer value
    std::unordered_map<std::string, int (*)(int, int)>::mapped_type op;
};

/**
 * @brief A class representing an effect that changes an attribute using a fixed float value
 */
class FloatNumEffect : public Effect {
public:
    /**
     * @brief Constructs a FloatNumEffect with the attribute it affects, the name of the operation, its execution time,
     * and the float used for calculation.
     * @param affected_attribute the name of the attribute whose calculation is affected by this effect
     * @param op_name the string representation of the operation that is performed when applying this effect
     * @param time the time at which this effect should be applied in the order of execution
     * @param value the fixed float value used in the operation
     */
    FloatNumEffect(
        const std::string& affected_attribute, const std::string& op_name, const EffectTime time, float value
    );
    /**
     * @brief Applies the float effect to a character's attributes given the attributes and constants of the character
     * @param attributes character attributes may be used for calculation and one of them will be changed
     * @param constants character constants may be used for calculation
     */
    virtual void applyTo(
        std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const override;

    // the fixed float value used in the operation
    const float value;
protected:
    // the operation to be applied to the attribute using the fixed float value
    std::unordered_map<std::string, int (*)(int, float)>::mapped_type op;
};

/**
 * @brief A class representing an effect that changes an attribute using an identifier i.e an attribute or constant
 */
class IdentifierEffect : public Effect {
public:
    /**
     * @brief Constructs an IdentifierEffect with the attribute it affects, the name of the operation,
     * its execution time, and the float used for calculation.
     * @param affected_attribute the name of the attribute whose calculation is affected by this effect
     * @param op_name the string representation of the operation that is performed when applying this effect
     * @param time the time at which this effect should be applied in the order of execution
     * @param identifier the name of the attribute or constant used in the operation
     */
    IdentifierEffect(
        const std::string& affected_attribute, const std::string& op_name, const EffectTime time,
        const std::string& identifier
    );

    // the name of the attribute or constant used in the operation
    const std::string identifier;
protected:
    // the operation to be applied to the attribute using the identifier value
    std::unordered_map<std::string, int (*)(int, int)>::mapped_type op;
};

/**
 * @brief A class representing an effect that changes an attribute using an attribute (usually a different attribute)
 */
class OtherAttributeEffect : public IdentifierEffect {
public:
    /**
     * @brief Constructs an IdentifierEffect with the attribute it affects, the name of the operation,
     * its execution time, and the float used for calculation.
     * @param affected_attribute the name of the attribute whose calculation is affected by this effect
     * @param op_name the string representation of the operation that is performed when applying this effect
     * @param time the time at which this effect should be applied in the order of execution
     * @param other_attribute the name of the attribute used in the operation
     */
    OtherAttributeEffect(
        const std::string& affected_attribute, const std::string& op_name, const EffectTime time,
        const std::string& other_attribute
    );
    /**
     * @brief Applies the effect to a character's attributes given the attributes and constants of the character
     * @param attributes character attributes may be used for calculation and one of them will be changed
     * @param constants character constants may be used for calculation
     */
    virtual void applyTo(
        std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const override;
};

/**
 * @brief A class representing an effect that changes an attribute using a constant
 */
class ConstEffect : public IdentifierEffect {
public:
    /**
     * @brief Constructs an IdentifierEffect with the attribute it affects, the name of the operation,
     * its execution time, and the float used for calculation.
     * @param affected_attribute the name of the attribute whose calculation is affected by this effect
     * @param op_name the string representation of the operation that is performed when applying this effect
     * @param time the time at which this effect should be applied in the order of execution
     * @param constant the name of the constant used in the operation
     */
    ConstEffect(
        const std::string& affected_attribute, const std::string& op_name, const EffectTime time,
        const std::string& constant
    );
    /**
     * @brief Applies the effect to a character's attributes given the attributes and constants of the character
     * @param attributes character attributes may be used for calculation and one of them will be changed
     * @param constants character constants may be used for calculation
     */
    virtual void applyTo(
        std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const override;
};


inline Effect::Effect(const std::string& affected_attribute, const std::string& op_name, const EffectTime time) noexcept
    : affected_attribute(affected_attribute), op_name(op_name), time(time) {}

inline IntNumEffect::IntNumEffect(
    const std::string& affected_attribute, const std::string& op_name, const EffectTime time, int value
)
    : Effect(affected_attribute, op_name, time), value(value), op(nullptr) {
    for (const auto& [operator_name, operator_func] : int_operators) {
        if (operator_name == op_name) {
            op = operator_func;
            return;
        }
    }
    throw std::out_of_range("Integer Effect Operator \"" + op_name + "\" does not exist.");
}

inline void IntNumEffect::applyTo(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    DND_UNUSED(constants);
    attributes[affected_attribute] = op(attributes[affected_attribute], value);
}

inline FloatNumEffect::FloatNumEffect(
    const std::string& affected_attribute, const std::string& op_name, const EffectTime time, float value
)
    : Effect(affected_attribute, op_name, time), value(value) {
    for (const auto& [operator_name, operator_func] : float_operators) {
        if (operator_name == op_name) {
            op = operator_func;
            return;
        }
    }
    throw std::out_of_range("Float Effect Operator \"" + op_name + "\" does not exist.");
}

inline void FloatNumEffect::applyTo(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    DND_UNUSED(constants);
    attributes[affected_attribute] = op(attributes[affected_attribute], value);
}

inline IdentifierEffect::IdentifierEffect(
    const std::string& affected_attribute, const std::string& op_name, const EffectTime time,
    const std::string& identifier
)
    : Effect(affected_attribute, op_name, time), identifier(identifier), op(nullptr) {
    for (const auto& [operator_name, operator_func] : int_operators) {
        if (operator_name == op_name) {
            op = operator_func;
            return;
        }
    }
    throw std::out_of_range("Integer Effect Operator \"" + op_name + "\" does not exist.");
}

inline OtherAttributeEffect::OtherAttributeEffect(
    const std::string& affected_attribute, const std::string& op_name, const EffectTime time,
    const std::string& other_attribute
)
    : IdentifierEffect(affected_attribute, op_name, time, other_attribute) {}

inline void OtherAttributeEffect::applyTo(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    try {
        DND_UNUSED(constants);
        attributes[affected_attribute] = op(attributes[affected_attribute], attributes.at(identifier));
    } catch (const std::out_of_range& e) {
        DND_UNUSED(e);
        throw std::out_of_range("Other attribute \"" + identifier + "\" does not exist.");
    }
}

inline ConstEffect::ConstEffect(
    const std::string& affected_attribute, const std::string& op_name, const EffectTime time,
    const std::string& constant
)
    : IdentifierEffect(affected_attribute, op_name, time, constant) {}

inline void ConstEffect::applyTo(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    try {
        attributes[affected_attribute] = op(attributes[affected_attribute], constants.at(identifier));
    } catch (const std::out_of_range& e) {
        DND_UNUSED(e);
        throw std::out_of_range("Constant \"" + identifier + "\" does not exist.");
    }
}

} // namespace dnd

#endif // EFFECT_HPP_
