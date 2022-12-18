#ifndef EFFECT_HPP_
#define EFFECT_HPP_

#include <stdexcept>

namespace dnd {

class Effect {
public:
    virtual void applyTo(int& value) const = 0;
};

class AddEffect : public Effect {
private:
    const int add;
public:
    AddEffect(int add) : add(add) {}
    void applyTo(int& value) const { value += add; }
};

class MultEffect : public Effect {
private:
    const float factor;
public:
    MultEffect(float factor) : factor(factor) {}
    void applyTo(int& value) const { value *= factor; }
};

class DivEffect : public Effect {
private:
    const float div;
public:
    DivEffect(float div) : div(div) {
        if (div == 0) {
            throw std::invalid_argument("Cannot divide by zero.");
        }
    }
    void applyTo(int& value) const { value /= div; }
};

class SetEffect : public Effect {
private:
    const int set_value;
public:
    SetEffect(int set_value) : set_value(set_value) {}
    void applyTo(int& value) const { value = set_value; }
};

} // namespace dnd

#endif // EFFECT_HPP_
