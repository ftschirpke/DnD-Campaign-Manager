#ifndef EFFECT_HPP_
#define EFFECT_HPP_

#include <stdexcept>
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

class Effect {
public:
    const std::string affected_attribute;
    Effect(const std::string& affected_attribute);
    virtual void apply(
        std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const = 0;
};

class AddEffect : public Effect {
public:
    const int add;
    AddEffect(const std::string& affected_attribute, int add);
    void apply(std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants)
        const;
};

class MultEffect : public Effect {
public:
    const float factor;
    MultEffect(const std::string& affected_attribute, float factor);
    void apply(std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants)
        const;
};

class DivEffect : public Effect {
public:
    const float div;
    DivEffect(const std::string& affected_attribute, float div);
    void apply(std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants)
        const;
};

class SetEffect : public Effect {
public:
    const int set_value;
    SetEffect(const std::string& affected_attribute, int set_value);
    void apply(std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants)
        const;
};

class AddOtherEffect : public Effect {
public:
    const std::string other_attribute;
    AddOtherEffect(const std::string& affected_attribute, const std::string& other_attribute);
    void apply(std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants)
        const;
};

class MultOtherEffect : public Effect {
public:
    const std::string other_attribute;
    MultOtherEffect(const std::string& affected_attribute, const std::string& other_attribute);
    void apply(std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants)
        const;
};

class DivOtherEffect : public Effect {
public:
    const std::string other_attribute;
    DivOtherEffect(const std::string& affected_attribute, const std::string& other_attribute);
    void apply(std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants)
        const;
};

class SetOtherEffect : public Effect {
public:
    const std::string other_attribute;
    SetOtherEffect(const std::string& affected_attribute, const std::string& other_attribute);
    void apply(std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants)
        const;
};

class AddConstEffect : public Effect {
public:
    const std::string constant;
    AddConstEffect(const std::string& affected_attribute, const std::string& constant);
    void apply(std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants)
        const;
};

class MultConstEffect : public Effect {
public:
    const std::string constant;
    MultConstEffect(const std::string& affected_attribute, const std::string& constant);
    void apply(std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants)
        const;
};

class DivConstEffect : public Effect {
public:
    const std::string constant;
    DivConstEffect(const std::string& affected_attribute, const std::string& constant);
    void apply(std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants)
        const;
};

class SetConstEffect : public Effect {
public:
    const std::string constant;
    SetConstEffect(const std::string& affected_attribute, const std::string& constant);
    void apply(std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants)
        const;
};


inline Effect::Effect(const std::string& affected_attribute) : affected_attribute(affected_attribute) {}

inline AddEffect::AddEffect(const std::string& affected_attribute, int add) : Effect(affected_attribute), add(add) {}

inline void AddEffect::apply(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    attributes[affected_attribute] += add;
}

inline MultEffect::MultEffect(const std::string& affected_attribute, float factor)
    : Effect(affected_attribute), factor(factor) {}

inline void MultEffect::apply(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    attributes[affected_attribute] *= factor;
}

inline DivEffect::DivEffect(const std::string& affected_attribute, float div) : Effect(affected_attribute), div(div) {
    if (div == 0) {
        throw std::invalid_argument("Cannot divide by zero.");
    }
}

inline void DivEffect::apply(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    attributes[affected_attribute] /= div;
}

inline SetEffect::SetEffect(const std::string& affected_attribute, int set_value)
    : Effect(affected_attribute), set_value(set_value) {}

inline void SetEffect::apply(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    attributes[affected_attribute] = set_value;
}

inline AddOtherEffect::AddOtherEffect(const std::string& affected_attribute, const std::string& other_attribute)
    : Effect(affected_attribute), other_attribute(other_attribute) {}

inline void AddOtherEffect::apply(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    try {
        attributes[affected_attribute] += attributes.at(other_attribute);
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("Other attribute \"" + other_attribute + "\" does not exist.");
    }
}

inline MultOtherEffect::MultOtherEffect(const std::string& affected_attribute, const std::string& other_attribute)
    : Effect(affected_attribute), other_attribute(other_attribute) {}

inline void MultOtherEffect::apply(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    try {
        attributes[affected_attribute] *= attributes.at(other_attribute) / 100.0f;
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("Other attribute \"" + other_attribute + "\" does not exist.");
    }
}

inline DivOtherEffect::DivOtherEffect(const std::string& affected_attribute, const std::string& other_attribute)
    : Effect(affected_attribute), other_attribute(other_attribute) {}

inline void DivOtherEffect::apply(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    try {
        attributes[affected_attribute] /= attributes.at(other_attribute) / 100.0f;
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("Other attribute \"" + other_attribute + "\" does not exist.");
    }
}

inline SetOtherEffect::SetOtherEffect(const std::string& affected_attribute, const std::string& other_attribute)
    : Effect(affected_attribute), other_attribute(other_attribute) {}

inline void SetOtherEffect::apply(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    try {
        attributes[affected_attribute] = attributes.at(other_attribute);
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("Other attribute \"" + other_attribute + "\" does not exist.");
    }
}

inline AddConstEffect::AddConstEffect(const std::string& affected_attribute, const std::string& constant)
    : Effect(affected_attribute), constant(constant) {}

inline void AddConstEffect::apply(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    try {
        attributes[affected_attribute] += constants.at(constant);
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("Constant \"" + constant + "\" does not exist.");
    }
}

inline MultConstEffect::MultConstEffect(const std::string& affected_attribute, const std::string& constant)
    : Effect(affected_attribute), constant(constant) {}

inline void MultConstEffect::apply(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    try {
        attributes[affected_attribute] *= constants.at(constant) / 100.0f;
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("Constant \"" + constant + "\" does not exist.");
    }
}

inline DivConstEffect::DivConstEffect(const std::string& affected_attribute, const std::string& constant)
    : Effect(affected_attribute), constant(constant) {}
inline void DivConstEffect::apply(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    try {
        attributes[affected_attribute] /= constants.at(constant) / 100.0f;
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("Constant \"" + constant + "\" does not exist.");
    }
}

inline SetConstEffect::SetConstEffect(const std::string& affected_attribute, const std::string& constant)
    : Effect(affected_attribute), constant(constant) {}

inline void SetConstEffect::apply(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    try {
        attributes[affected_attribute] = constants.at(constant);
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("Constant \"" + constant + "\" does not exist.");
    }
}

} // namespace dnd

#endif // EFFECT_HPP_
