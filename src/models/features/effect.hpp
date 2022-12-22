#ifndef EFFECT_HPP_
#define EFFECT_HPP_

#include <stdexcept>

#include "models/creature_state.hpp"

namespace dnd {

class Effect {
public:
    const std::string affected_attribute;
    Effect(const std::string& affected_attribute);
    virtual void applyTo(CreatureState& state) const = 0;
};

class AddEffect : public Effect {
public:
    const int add;
    AddEffect(const std::string& affected_attribute, int add);
    void applyTo(CreatureState& state) const;
};

class MultEffect : public Effect {
public:
    const float factor;
    MultEffect(const std::string& affected_attribute, float factor);
    void applyTo(CreatureState& state) const;
};

class DivEffect : public Effect {
public:
    const float div;
    DivEffect(const std::string& affected_attribute, float div);
    void applyTo(CreatureState& state) const;
};

class SetEffect : public Effect {
public:
    const int set_value;
    SetEffect(const std::string& affected_attribute, int set_value);
    void applyTo(CreatureState& state) const;
};

class AddOtherEffect : public Effect {
public:
    const std::string other_attribute;
    AddOtherEffect(const std::string& affected_attribute, const std::string& other_attribute);
    void applyTo(CreatureState& state) const;
};

class MultOtherEffect : public Effect {
public:
    const std::string other_attribute;
    MultOtherEffect(const std::string& affected_attribute, const std::string& other_attribute);
    void applyTo(CreatureState& state) const;
};

class DivOtherEffect : public Effect {
public:
    const std::string other_attribute;
    DivOtherEffect(const std::string& affected_attribute, const std::string& other_attribute);
    void applyTo(CreatureState& state) const;
};

class SetOtherEffect : public Effect {
public:
    const std::string other_attribute;
    SetOtherEffect(const std::string& affected_attribute, const std::string& other_attribute);
    void applyTo(CreatureState& state) const;
};

class AddConstEffect : public Effect {
public:
    const std::string constant;
    AddConstEffect(const std::string& affected_attribute, const std::string& constant);
    void applyTo(CreatureState& state) const;
};

class MultConstEffect : public Effect {
public:
    const std::string constant;
    MultConstEffect(const std::string& affected_attribute, const std::string& constant);
    void applyTo(CreatureState& state) const;
};

class DivConstEffect : public Effect {
public:
    const std::string constant;
    DivConstEffect(const std::string& affected_attribute, const std::string& constant);
    void applyTo(CreatureState& state) const;
};

class SetConstEffect : public Effect {
public:
    const std::string constant;
    SetConstEffect(const std::string& affected_attribute, const std::string& constant);
    void applyTo(CreatureState& state) const;
};


inline Effect::Effect(const std::string& affected_attribute) : affected_attribute(affected_attribute) {}

inline AddEffect::AddEffect(const std::string& affected_attribute, int add) : Effect(affected_attribute), add(add) {}

inline void AddEffect::applyTo(CreatureState& state) const { state.attributes[affected_attribute] += add; }

inline MultEffect::MultEffect(const std::string& affected_attribute, float factor)
    : Effect(affected_attribute), factor(factor) {}

inline void MultEffect::applyTo(CreatureState& state) const { state.attributes[affected_attribute] *= factor; }

inline DivEffect::DivEffect(const std::string& affected_attribute, float div) : Effect(affected_attribute), div(div) {
    if (div == 0) {
        throw std::invalid_argument("Cannot divide by zero.");
    }
}

inline void DivEffect::applyTo(CreatureState& state) const { state.attributes[affected_attribute] /= div; }

inline SetEffect::SetEffect(const std::string& affected_attribute, int set_value)
    : Effect(affected_attribute), set_value(set_value) {}

inline void SetEffect::applyTo(CreatureState& state) const { state.attributes[affected_attribute] = set_value; }

inline AddOtherEffect::AddOtherEffect(const std::string& affected_attribute, const std::string& other_attribute)
    : Effect(affected_attribute), other_attribute(other_attribute) {}

inline void AddOtherEffect::applyTo(CreatureState& state) const {
    try {
        state.attributes[affected_attribute] += state.attributes.at(other_attribute);
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("Other attribute \"" + other_attribute + "\" does not exist.");
    }
}

inline MultOtherEffect::MultOtherEffect(const std::string& affected_attribute, const std::string& other_attribute)
    : Effect(affected_attribute), other_attribute(other_attribute) {}

inline void MultOtherEffect::applyTo(CreatureState& state) const {
    try {
        state.attributes[affected_attribute] *= state.attributes.at(other_attribute) / 100.0f;
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("Other attribute \"" + other_attribute + "\" does not exist.");
    }
}

inline DivOtherEffect::DivOtherEffect(const std::string& affected_attribute, const std::string& other_attribute)
    : Effect(affected_attribute), other_attribute(other_attribute) {}

inline void DivOtherEffect::applyTo(CreatureState& state) const {
    try {
        state.attributes[affected_attribute] /= state.attributes.at(other_attribute) / 100.0f;
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("Other attribute \"" + other_attribute + "\" does not exist.");
    }
}

inline SetOtherEffect::SetOtherEffect(const std::string& affected_attribute, const std::string& other_attribute)
    : Effect(affected_attribute), other_attribute(other_attribute) {}

inline void SetOtherEffect::applyTo(CreatureState& state) const {
    try {
        state.attributes[affected_attribute] = state.attributes.at(other_attribute);
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("Other attribute \"" + other_attribute + "\" does not exist.");
    }
}

inline AddConstEffect::AddConstEffect(const std::string& affected_attribute, const std::string& constant)
    : Effect(affected_attribute), constant(constant) {}

inline void AddConstEffect::applyTo(CreatureState& state) const {
    try {
        state.attributes[affected_attribute] += state.constants.at(constant);
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("Constant \"" + constant + "\" does not exist.");
    }
}

inline MultConstEffect::MultConstEffect(const std::string& affected_attribute, const std::string& constant)
    : Effect(affected_attribute), constant(constant) {}

inline void MultConstEffect::applyTo(CreatureState& state) const {
    try {
        state.attributes[affected_attribute] *= state.constants.at(constant) / 100.0f;
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("Constant \"" + constant + "\" does not exist.");
    }
}

inline DivConstEffect::DivConstEffect(const std::string& affected_attribute, const std::string& constant)
    : Effect(affected_attribute), constant(constant) {}
inline void DivConstEffect::applyTo(CreatureState& state) const {
    try {
        state.attributes[affected_attribute] /= state.constants.at(constant) / 100.0f;
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("Constant \"" + constant + "\" does not exist.");
    }
}

inline SetConstEffect::SetConstEffect(const std::string& affected_attribute, const std::string& constant)
    : Effect(affected_attribute), constant(constant) {}

inline void SetConstEffect::applyTo(CreatureState& state) const {
    try {
        state.attributes[affected_attribute] = state.constants.at(constant);
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("Constant \"" + constant + "\" does not exist.");
    }
}

} // namespace dnd

#endif // EFFECT_HPP_
