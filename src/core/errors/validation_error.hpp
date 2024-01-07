#ifndef VALIDATION_ERROR_HPP_
#define VALIDATION_ERROR_HPP_

#include <dnd_config.hpp>

#include <string>

namespace dnd {

class ValidationData;

class ValidationError {
public:
    enum class Code;

    ValidationError(Code error_code, const std::string& message) noexcept;

    Code get_error_code() const noexcept;
    const std::string& get_error_message() const noexcept;
private:
    Code error_code;
    std::string error_message;
};

enum class ValidationError::Code {
    // a required value was not found
    MISSING_ATTRIBUTE,
    // the format of an attribute was invalid
    INVALID_ATTRIBUTE_FORMAT,
    // found an attribute with invalid value
    INVALID_ATTRIBUTE_VALUE,
    // found inconsistencies between attributes
    INCONSISTENT_ATTRIBUTES,
    // a relation to another content piece was not found
    RELATION_NOT_FOUND,
    // a relation to another content piece is invalid
    INVALID_RELATION,
    // an unknown error occurred
    UNKNOWN_ERROR,
};

} // namespace dnd

#endif // VALIDATION_ERROR_HPP_
