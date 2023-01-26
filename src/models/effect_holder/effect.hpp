#ifndef EFFECT_HPP_
#define EFFECT_HPP_

#include <algorithm>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace dnd {

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

const std::vector<EffectTime> effect_times_in_order = {
    EffectTime::EARLIEST, EffectTime::EARLY, EffectTime::NORMAL, EffectTime::LATE, EffectTime::LATEST,
};

const std::unordered_map<std::string, EffectTime> effect_time_for_string = {
    {"earliest", EffectTime::EARLIEST}, {"early", EffectTime::EARLY},   {"normal", EffectTime::NORMAL},
    {"late", EffectTime::LATE},         {"latest", EffectTime::LATEST},
};

const std::unordered_map<std::string, int (*)(int, int)> int_effect_operators = {
    {"add", [](int a, int b) { return a + b; }},
    {"mult", [](int a, int b) { return (int)(a * b / 100.0f); }},
    {
        "div",
        [](int a, int b) {
            if (b == 0) {
                throw std::invalid_argument("Cannot divide by zero.");
            }
            return (int)(a / (b / 100.0f));
        },
    },
    {"set", [](int a, int b) { return b; }},
    {"max", [](int a, int b) { return std::max(a, b); }},
    {"min", [](int a, int b) { return std::min(a, b); }},
};

const std::unordered_map<std::string, int (*)(int, float)> float_effect_operators = {
    {"mult", [](int a, float b) { return (int)(a * b); }},
    {
        "div",
        [](int a, float b) {
            if (b == 0) {
                throw std::invalid_argument("Cannot divide by zero.");
            }
            return (int)(a / b);
        },
    },
};

class Effect {
public:
    const std::string affected_attribute, op_name;
    const EffectTime time;
    Effect(const std::string& affected_attribute, const std::string& op_name, const EffectTime time) noexcept;
    virtual void applyTo(
        std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const = 0;
};

class IntNumEffect : public Effect {
public:
    const int value;
    IntNumEffect(const std::string& affected_attribute, const std::string& op_name, const EffectTime time, int value);
    virtual void applyTo(
        std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const;
protected:
    std::unordered_map<std::string, int (*)(int, int)>::mapped_type op;
};

class FloatNumEffect : public Effect {
public:
    const float value;
    FloatNumEffect(
        const std::string& affected_attribute, const std::string& op_name, const EffectTime time, float value
    );
    virtual void applyTo(
        std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const;
protected:
    std::unordered_map<std::string, int (*)(int, float)>::mapped_type op;
};

class IdentifierEffect : public Effect {
public:
    const std::string identifier;
    IdentifierEffect(
        const std::string& affected_attribute, const std::string& op_name, const EffectTime time,
        const std::string& identifier
    );
protected:
    std::unordered_map<std::string, int (*)(int, int)>::mapped_type op;
};

class OtherAttributeEffect : public IdentifierEffect {
public:
    OtherAttributeEffect(
        const std::string& affected_attribute, const std::string& op_name, const EffectTime time,
        const std::string& identifier
    );
    virtual void applyTo(
        std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const;
};

class ConstEffect : public IdentifierEffect {
public:
    ConstEffect(
        const std::string& affected_attribute, const std::string& op_name, const EffectTime time,
        const std::string& identifier
    );
    virtual void applyTo(
        std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const;
};


inline Effect::Effect(const std::string& affected_attribute, const std::string& op_name, const EffectTime time) noexcept
    : affected_attribute(affected_attribute), op_name(op_name), time(time) {}

inline IntNumEffect::IntNumEffect(
    const std::string& affected_attribute, const std::string& op_name, const EffectTime time, int value
)
    : Effect(affected_attribute, op_name, time), value(value), op(nullptr) {
    try {
        op = int_effect_operators.at(op_name);
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("Operator \"" + op_name + "\" does not exist.");
    }
}

inline void IntNumEffect::applyTo(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    attributes[affected_attribute] = int_effect_operators.at(op_name)(attributes[affected_attribute], value);
}

inline FloatNumEffect::FloatNumEffect(
    const std::string& affected_attribute, const std::string& op_name, const EffectTime time, float value
)
    : Effect(affected_attribute, op_name, time), value(value) {
    try {
        op = float_effect_operators.at(op_name);
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("Operator \"" + op_name + "\" does not exist.");
    }
}

inline void FloatNumEffect::applyTo(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    attributes[affected_attribute] = float_effect_operators.at(op_name)(attributes[affected_attribute], value);
}

inline IdentifierEffect::IdentifierEffect(
    const std::string& affected_attribute, const std::string& op_name, const EffectTime time,
    const std::string& identifier
)
    : Effect(affected_attribute, op_name, time), identifier(identifier), op(nullptr) {
    try {
        op = int_effect_operators.at(op_name);
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("Operator \"" + op_name + "\" does not exist.");
    }
}

inline OtherAttributeEffect::OtherAttributeEffect(
    const std::string& affected_attribute, const std::string& op_name, const EffectTime time,
    const std::string& identifier
)
    : IdentifierEffect(affected_attribute, op_name, time, identifier) {}

inline void OtherAttributeEffect::applyTo(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    try {
        attributes[affected_attribute] = op(attributes[affected_attribute], attributes.at(identifier));
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("Other attribute \"" + identifier + "\" does not exist.");
    }
}

inline ConstEffect::ConstEffect(
    const std::string& affected_attribute, const std::string& op_name, const EffectTime time,
    const std::string& identifier
)
    : IdentifierEffect(affected_attribute, op_name, time, identifier) {}

inline void ConstEffect::applyTo(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    try {
        attributes[affected_attribute] = op(attributes[affected_attribute], constants.at(identifier));
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("Constant \"" + identifier + "\" does not exist.");
    }
}

} // namespace dnd

#endif // EFFECT_HPP_
