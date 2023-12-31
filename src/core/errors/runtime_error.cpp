#include <dnd_config.hpp>

#include "runtime_error.hpp"

#include <string>
#include <utility>

namespace dnd {

RuntimeError::RuntimeError(Code code, const std::string& message) noexcept : code(code), error_message(message) {}

RuntimeError::RuntimeError(Code code, std::string&& message) noexcept : code(code), error_message(std::move(message)) {}

RuntimeError::Code RuntimeError::get_error_code() const noexcept { return code; }

const std::string& RuntimeError::get_message() const noexcept { return error_message; }

} // namespace dnd
