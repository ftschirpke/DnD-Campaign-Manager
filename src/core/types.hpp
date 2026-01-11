#ifndef TYPES_HPP_
#define TYPES_HPP_

#include <functional>
#include <optional>
#include <variant>

#include <x/content_pieces.hpp>

namespace dnd {

enum class Type {
#define X(C, u, j, a, p, P) C,
    X_CONTENT_PIECES
#undef X
};

struct Id {
    std::strong_ordering operator<=>(const Id&) const = default;

    size_t index;
    Type type;
};

#define dispatch(var, el, func_body, ...) std::visit([&](el) { return func_body; }, var);

// a shorthand for std::reference_wrapper<T>
template <typename T>
using Ref = std::reference_wrapper<T>;

// a shorthand for a std::reference_wrapper<const T>
template <typename T>
using CRef = std::reference_wrapper<const T>;

// a shorthand for std::optional
template <typename T>
using Opt = std::optional<T>;

// a variant of references to T
template <typename... T>
using VarOfRef = std::variant<std::reference_wrapper<T>...>;

// a variant of references to const T
template <typename... T>
using VarOfCRef = std::variant<std::reference_wrapper<const T>...>;

} // namespace dnd

#endif // TYPES_HPP_
