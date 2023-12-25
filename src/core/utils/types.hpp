#ifndef TYPES_HPP_
#define TYPES_HPP_

#include <functional>
#include <optional>

namespace dnd {

// an optional reference to a T
template <typename T>
using OptRef = std::optional<std::reference_wrapper<T>>;

// an optional reference to a T
template <typename T>
using OptCRef = std::optional<std::reference_wrapper<const T>>;

} // namespace dnd

#endif // TYPES_HPP_
