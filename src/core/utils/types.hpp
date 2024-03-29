#ifndef TYPES_HPP_
#define TYPES_HPP_

#include <functional>
#include <optional>
#include <variant>

namespace dnd {

// a shorthand for std::reference_wrapper<T>
template <typename T>
using Ref = std::reference_wrapper<T>;

// a shorthand for a std::reference_wrapper<const T>
template <typename T>
using CRef = std::reference_wrapper<const T>;

// an optional reference to a T
template <typename T>
using OptRef = std::optional<std::reference_wrapper<T>>;

// an optional reference to a const T
template <typename T>
using OptCRef = std::optional<std::reference_wrapper<const T>>;

// a variant of references to T
template <typename... T>
using VarOfRef = std::variant<std::reference_wrapper<T>...>;

// a variant of references to const T
template <typename... T>
using VarOfCRef = std::variant<std::reference_wrapper<const T>...>;

} // namespace dnd

#endif // TYPES_HPP_
