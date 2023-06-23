#include <dnd_config.hpp>

#include "literal_effect.hpp"

#include <functional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

#include <core/models/effect_holder/effect/effect.hpp>

dnd::LiteralEffect::LiteralEffect(
    const std::string& affected_attribute, EffectTime time, const std::string& operation_name, bool value
) noexcept
    : Effect(affected_attribute, time, operation_name), value(static_cast<int>(value)) {}

dnd::LiteralEffect::LiteralEffect(
    std::string_view affected_attribute, EffectTime time, std::string_view operation_name, bool value
) noexcept
    : Effect(affected_attribute, time, operation_name), value(static_cast<int>(value)) {}

dnd::LiteralEffect::LiteralEffect(
    const std::string& affected_attribute, EffectTime time, const std::string& operation_name, int value
) noexcept
    : Effect(affected_attribute, time, operation_name), value(value * 100) {}

dnd::LiteralEffect::LiteralEffect(
    std::string_view affected_attribute, EffectTime time, std::string_view operation_name, int value
) noexcept
    : Effect(affected_attribute, time, operation_name), value(value * 100) {}

dnd::LiteralEffect::LiteralEffect(
    const std::string& affected_attribute, EffectTime time, const std::string& operation_name, float value
) noexcept
    : Effect(affected_attribute, time, operation_name), value(static_cast<int>(value * 100)) {}

dnd::LiteralEffect::LiteralEffect(
    std::string_view affected_attribute, EffectTime time, std::string_view operation_name, float value
) noexcept
    : Effect(affected_attribute, time, operation_name), value(static_cast<int>(value * 100)) {}

void dnd::LiteralEffect::apply_to(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    DND_UNUSED(constants);
    if (mathematical_operation == nullptr) {
        return;
    }
    attributes[affected_attribute] = mathematical_operation(attributes[affected_attribute], value);
}
