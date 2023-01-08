#ifndef CHOOSABLE_HPP_
#define CHOOSABLE_HPP_

#include <memory>
#include <vector>

#include "models/character.hpp"
#include "models/effects_holder/effects_holder.hpp"
#include "models/effects_holder/prerequisite.hpp"

namespace dnd {

class Choosable : public EffectsHolder {
public:
    std::vector<std::unique_ptr<Prerequisite>> prerequisites;
    Choosable(Choosable&& other) = default;
    Choosable(EffectsHolder&& effects_holder);
    bool isAllowed(const Character& character) const;
};

inline Choosable::Choosable(EffectsHolder&& effects_holder) : EffectsHolder(std::move(effects_holder)) {}

} // namespace dnd

#endif // CHOOSABLE_HPP_
