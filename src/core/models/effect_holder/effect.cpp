#include "dnd_config.hpp"

#include "effect.hpp"

#include <array>
#include <utility>

constexpr std::array<std::pair<const char*, int (*)(int, int)>, 6> dnd::Effect::int_operators = {
    std::pair("add", [](int a, int b) { return a + b; }),
    std::pair("mult", [](int a, int b) { return static_cast<int>(a * b / 100.0f); }),
    {
        "div",
        [](int a, int b) {
            if (b == 0) {
                throw std::invalid_argument("Cannot divide by zero.");
            }
            return static_cast<int>(a / (b / 100.0f));
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

constexpr std::array<std::pair<const char*, int (*)(int, float)>, 2> dnd::Effect::float_operators = {
    std::pair("mult", [](int a, float b) { return static_cast<int>(a * b); }),
    std::pair(
        "div",
        [](int a, float b) {
            if (b == 0) {
                throw std::invalid_argument("Cannot divide by zero.");
            }
            return static_cast<int>(a / b);
        }
    ),
};
