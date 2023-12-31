#ifndef RUNTIME_ERROR_HPP_
#define RUNTIME_ERROR_HPP_

#include <dnd_config.hpp>

#include <string>

namespace dnd {

class RuntimeError {
public:
    enum class Code;

    RuntimeError(Code code, const std::string& message) noexcept;
    RuntimeError(Code code, std::string&& message) noexcept;

    Code get_error_code() const noexcept;
    const std::string& get_message() const noexcept;
private:
    Code code;
    std::string error_message;
};

enum class RuntimeError::Code {
    // function was provided with invalid argument
    INVALID_ARGUMENT,
    // this code should never be reached
    UNREACHABLE,
    // an unknown error occurred
    UNKNOWN_ERROR,
};

} // namespace dnd

#endif // RUNTIME_ERROR_HPP_
