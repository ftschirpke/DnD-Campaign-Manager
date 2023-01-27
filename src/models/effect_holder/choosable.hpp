#ifndef CHOOSABLE_HPP_
#define CHOOSABLE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "models/effect_holder/effect_holder.hpp"
#include "models/effect_holder/prerequisite.hpp"

namespace dnd {

class Choosable {
public:
    const std::string name, description;
    std::vector<std::unique_ptr<Prerequisite>> prerequisites;
    EffectHolder main_part;
    std::vector<EffectHolder> parts;
    Choosable(const std::string& name, const std::string& description) noexcept;
    Choosable(Choosable&& other) noexcept = default;
};

inline Choosable::Choosable(const std::string& name, const std::string& description) noexcept
    : name(name), description(description) {}

} // namespace dnd

#endif // CHOOSABLE_HPP_
