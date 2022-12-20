#ifndef EFFECT_HPP_
#define EFFECT_HPP_

#include <stdexcept>

#include "models/creature_state.hpp"

namespace dnd {

class Effect {
public:
    const std::string affected_attribute;
    Effect(const std::string& affected_attribute) : affected_attribute(affected_attribute) {}
    virtual void applyTo(CreatureState& state) const = 0;
};

class AddEffect : public Effect {
public:
    const int add;
    AddEffect(const std::string& affected_attribute, int add) : Effect(affected_attribute), add(add) {}
    void applyTo(CreatureState& state) const { state.attributes[affected_attribute] += add; }
};

class MultEffect : public Effect {
public:
    const float factor;
    MultEffect(const std::string& affected_attribute, float factor) : Effect(affected_attribute), factor(factor) {}
    void applyTo(CreatureState& state) const { state.attributes[affected_attribute] *= factor; }
};

class DivEffect : public Effect {
public:
    const float div;
    DivEffect(const std::string& affected_attribute, float div) : Effect(affected_attribute), div(div) {
        if (div == 0) {
            throw std::invalid_argument("Cannot divide by zero.");
        }
    }
    void applyTo(CreatureState& state) const { state.attributes[affected_attribute] /= div; }
};

class SetEffect : public Effect {
public:
    const int set_value;
    SetEffect(const std::string& affected_attribute, int set_value)
        : Effect(affected_attribute), set_value(set_value) {}
    void applyTo(CreatureState& state) const { state.attributes[affected_attribute] = set_value; }
};

class AddOtherEffect : public Effect {
public:
    const std::string other_attribute;
    AddOtherEffect(const std::string& affected_attribute, const std::string& other_attribute)
        : Effect(affected_attribute), other_attribute(other_attribute) {}
    void applyTo(CreatureState& state) const {
        try {
            state.attributes[affected_attribute] += state.attributes.at(other_attribute);
        } catch (const std::out_of_range& e) {
            throw std::out_of_range("Other attribute \"" + other_attribute + "\" does not exist.");
        }
    }
};

class MultOtherEffect : public Effect {
public:
    const std::string other_attribute;
    MultOtherEffect(const std::string& affected_attribute, const std::string& other_attribute)
        : Effect(affected_attribute), other_attribute(other_attribute) {}
    void applyTo(CreatureState& state) const {
        try {
            state.attributes[affected_attribute] *= state.attributes.at(other_attribute) / 100.0f;
        } catch (const std::out_of_range& e) {
            throw std::out_of_range("Other attribute \"" + other_attribute + "\" does not exist.");
        }
    }
};

class DivOtherEffect : public Effect {
public:
    const std::string other_attribute;
    DivOtherEffect(const std::string& affected_attribute, const std::string& other_attribute)
        : Effect(affected_attribute), other_attribute(other_attribute) {}
=======
    SetEffect(const std::string& affected_attribute, int set_value) :
        Effect(affected_attribute), set_value(set_value) {}
    void applyTo(CreatureState& state) const {
        state.attributes[affected_attribute] = set_value;
    }
};

class AddOtherEffect : public Effect {
public:
    const std::string other_attribute;
    AddOtherEffect(const std::string& affected_attribute, const std::string& other_attribute) :
        Effect(affected_attribute), other_attribute(other_attribute) {}
    void applyTo(CreatureState& state) const {
        try {
            state.attributes[affected_attribute] += state.attributes.at(other_attribute);
        } catch (const std::out_of_range& e) {
            throw std::out_of_range("Other attribute \"" + other_attribute + "\" does not exist.");
        }
    }
};

class MultOtherEffect : public Effect {
public:
    const std::string other_attribute;
    MultOtherEffect(const std::string& affected_attribute, const std::string& other_attribute) :
        Effect(affected_attribute), other_attribute(other_attribute) {}
    void applyTo(CreatureState& state) const {
        try {
            state.attributes[affected_attribute] *= state.attributes.at(other_attribute) / 100.0f;
        } catch (const std::out_of_range& e) {
            throw std::out_of_range("Other attribute \"" + other_attribute + "\" does not exist.");
        }
    }
};

class DivOtherEffect : public Effect {
public:
    const std::string other_attribute;
    DivOtherEffect(const std::string& affected_attribute, const std::string& other_attribute) :
        Effect(affected_attribute), other_attribute(other_attribute) {}
    void applyTo(CreatureState& state) const {
        try {
            state.attributes[affected_attribute] /= state.attributes.at(other_attribute) / 100.0f;
        } catch (const std::out_of_range& e) {
            throw std::out_of_range("Other attribute \"" + other_attribute + "\" does not exist.");
        }
    }
};

class SetOtherEffect : public Effect {
public:
    const std::string other_attribute;
    SetOtherEffect(const std::string& affected_attribute, const std::string& other_attribute)
        : Effect(affected_attribute), other_attribute(other_attribute) {}
    void applyTo(CreatureState& state) const {
        try {
            state.attributes[affected_attribute] = state.attributes.at(other_attribute);
        } catch (const std::out_of_range& e) {
            throw std::out_of_range("Other attribute \"" + other_attribute + "\" does not exist.");
        }
    }
};

class AddConstEffect : public Effect {
public:
    const std::string constant;
    AddConstEffect(const std::string& affected_attribute, const std::string& constant)
        : Effect(affected_attribute), constant(constant) {}
    void applyTo(CreatureState& state) const {
        try {
            state.attributes[affected_attribute] += state.constants.at(constant);
        } catch (const std::out_of_range& e) {
            throw std::out_of_range("Constant \"" + constant + "\" does not exist.");
        }
    }
};

class MultConstEffect : public Effect {
public:
    const std::string constant;
    MultConstEffect(const std::string& affected_attribute, const std::string& constant)
        : Effect(affected_attribute), constant(constant) {}
    void applyTo(CreatureState& state) const {
        try {
            state.attributes[affected_attribute] *= state.constants.at(constant) / 100.0f;
        } catch (const std::out_of_range& e) {
            throw std::out_of_range("Constant \"" + constant + "\" does not exist.");
        }
    }
};

class DivConstEffect : public Effect {
public:
    const std::string constant;
    DivConstEffect(const std::string& affected_attribute, const std::string& constant)
        : Effect(affected_attribute), constant(constant) {}
    void applyTo(CreatureState& state) const {
        try {
            state.attributes[affected_attribute] /= state.constants.at(constant) / 100.0f;
        } catch (const std::out_of_range& e) {
            throw std::out_of_range("Constant \"" + constant + "\" does not exist.");
        }
    }
};

class SetConstEffect : public Effect {
public:
    const std::string constant;
    SetConstEffect(const std::string& affected_attribute, const std::string& constant)
        : Effect(affected_attribute), constant(constant) {}
    void applyTo(CreatureState& state) const {
        try {
            state.attributes[affected_attribute] = state.constants.at(constant);
        } catch (const std::out_of_range& e) {
            throw std::out_of_range("Constant \"" + constant + "\" does not exist.");
        }
    }
};

} // namespace dnd

#endif // EFFECT_HPP_
