#ifndef VALIDATION_ERROR_HPP_
#define VALIDATION_ERROR_HPP_

#include <dnd_config.hpp>

#include <string>

namespace dnd {

class ValidationData;

enum class ValidationErrorCode {
    // a required value was not found
    MISSING_ATTRIBUTE,
    // the format of an attribute was invalid
    INVALID_ATTRIBUTE_FORMAT,
    // found an attribute with invalid value
    INVALID_ATTRIBUTE_VALUE,
    // found inconsistencies between attributes
    INCONSISTENT_ATTRIBUTES,
    // an unknown error occurred
    UNKNOWN_ERROR,
};

class ValidationError {
public:
    ValidationError(
        ValidationErrorCode error_code, const ValidationData* validation_data, const std::string& message
    ) noexcept;

    ValidationErrorCode get_error_code() const noexcept;
    const ValidationData* get_validation_data() const noexcept;
    const std::string& get_error_message() const noexcept;
private:
    ValidationErrorCode error_code;
    const ValidationData* validation_data;
    std::string error_message;
};


} // namespace dnd

#endif // VALIDATION_ERROR_HPP_
