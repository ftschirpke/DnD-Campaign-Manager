#ifndef ERRORS_HPP_
#define ERRORS_HPP_

#include <dnd_config.hpp>

#include <utility>
#include <vector>

#include <core/errors/parsing_error.hpp>
#include <core/errors/validation_error.hpp>

namespace dnd {

class Errors {
public:
    /**
     * @brief Returns true if there are no errors.
     * @return true if there are no errors, false otherwise
     */
    bool ok() const;
    /**
     * @brief Adds a parsing error to the list of errors
     * @param error_code the parsing error code
     * @param filepath the path to the file where the error occurred
     * @param message the error message
     */
    void add_parsing_error(ParsingErrorCode error_code, std::filesystem::path&& filepath, std::string&& message);
    /**
     * @brief Adds a parsing error to the list of errors
     * @param error the parsing error
     */
    void add_parsing_error(ParsingError&& error);
    /**
     * @brief Adds a validation error to the list of errors
     * @param error_code the validation error code
     * @param validation_data a pointer to the validation data where the error occurred
     * @param message the error message
     */
    void add_validation_error(
        ValidationErrorCode error_code, const ValidationData* validation_data, std::string&& message
    );
    /**
     * @brief Adds a validation error to the list of errors
     * @param error the validation error
     */
    void add_validation_error(ValidationError&& error);
    /**
     * @brief Gets the list of parsing errors
     * @return the list of parsing errors
     */
    const std::vector<ParsingError>& get_parsing_errors() const;
    /**
     * @brief Gets the list of validation errors
     * @return the list of validation errors
     */
    const std::vector<ValidationError>& get_validation_errors() const;
    /**
     * @brief Merges the errors from another Errors object into this one
     * @param other the Errors object to merge
     */
    void merge(Errors&& other);
private:
    std::vector<ParsingError> parsing_errors;
    std::vector<ValidationError> validation_errors;
};

} // namespace dnd

#endif // ERRORS_HPP_
