#include <dnd_config.hpp>

#include "stat_change.hpp"

#include <array>
#include <functional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

namespace dnd {

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

StatChange::StatChange(const std::string& affected_attribute, StatChangeTime time, const std::string& operation_name)
    : affected_attribute(affected_attribute), mathematical_operation(nullptr), time(time) {
    for (const auto& [name, operation] : mathematical_operations) {
        if (name == operation_name) {
            mathematical_operation = operation;
            break;
        }
    }
}

StatChange::StatChange(std::string_view affected_attribute, StatChangeTime time, std::string_view operation_name)
    : affected_attribute(affected_attribute), mathematical_operation(nullptr), time(time) {
    for (const auto& [name, operation] : mathematical_operations) {
        if (name == operation_name) {
            mathematical_operation = operation;
            break;
        }
    }
}

StatChangeTime StatChange::get_time() const { return time; }

} // namespace dnd
