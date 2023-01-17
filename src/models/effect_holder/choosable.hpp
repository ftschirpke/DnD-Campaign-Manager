#ifndef CHOOSABLE_HPP_
#define CHOOSABLE_HPP_

#include <memory>
#include <vector>

#include "models/character.hpp"
#include "models/effect_holder/effect_holder.hpp"
#include "models/effect_holder/prerequisite.hpp"

namespace dnd {

class Choosable : public EffectHolder {
public:
    std::vector<std::unique_ptr<Prerequisite>> prerequisites;
    Choosable(Choosable&& other) noexcept = default;
    Choosable(EffectHolder&& effect_holder) noexcept;
    bool isAllowedFor(const Character& character) const;
};

inline Choosable::Choosable(EffectHolder&& effect_holder) noexcept : EffectHolder(std::move(effect_holder)) {}

} // namespace dnd

#endif // CHOOSABLE_HPP_
