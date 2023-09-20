#include <dnd_config.hpp>

#include "effect.hpp"

#include <array>
#include <functional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

static constexpr std::array<std::pair<const char*, int (*)(int, int)>, 7> mathematical_operations = {
    std::pair("add", [](int a, int b) { return a + b; }),
    std::pair("sub", [](int a, int b) { return a - b; }),
    std::pair("mult", [](int a, int b) { return static_cast<int>(a * b / 100.0f); }),
    {
        "div",
        [](int a, int b) {
            if (b == 0) {
                throw std::invalid_argument("Cannot divide by zero.");
            }
            return a * 100 / b;
        },
    },
    std::pair(
        "set",
        [](int a, int b) {
            DND_UNUSED(a);
            return b;
        }
    ),
    std::pair("max", [](int a, int b) { return std::max(a, b); }),
    std::pair("min", [](int a, int b) { return std::min(a, b); }),
};

dnd::Effect::Effect(
    const std::string& affected_attribute, dnd::EffectTime time, const std::string& operation_name
) noexcept
    : affected_attribute(affected_attribute), mathematical_operation(nullptr), time(time) {
    for (const auto& [name, operation] : mathematical_operations) {
        if (name == operation_name) {
            mathematical_operation = operation;
            break;
        }
    }
}

dnd::Effect::Effect(std::string_view affected_attribute, dnd::EffectTime time, std::string_view operation_name) noexcept
    : affected_attribute(affected_attribute), mathematical_operation(nullptr), time(time) {
    for (const auto& [name, operation] : mathematical_operations) {
        if (name == operation_name) {
            mathematical_operation = operation;
            break;
        }
    }
}

const std::string& dnd::Effect::get_affected_attribute() const noexcept { return affected_attribute; }

dnd::EffectTime dnd::Effect::get_time() const noexcept { return time; }
