#ifndef ERRORS_HPP_
#define ERRORS_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <string>
#include <vector>

#include <core/errors/parsing_error.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class Errors {
public:
    Errors() noexcept = default;
    Errors(const Errors&) = delete;
    Errors& operator=(const Errors&) = delete;
    Errors(Errors&&) noexcept = default;
    Errors& operator=(Errors&&) noexcept = default;

    /**
     * @brief Returns "true" if there are no errors.
     * @return "true" if there are no errors, "false" otherwise
     */
    bool ok() const;
    void add_parsing_error(ParsingErrorCode error_code, const std::filesystem::path& filepath, std::string&& message);
    void add_parsing_error(ParsingError&& error);
    void add_validation_error(
        ValidationErrorCode error_code, const ValidationData* validation_data, std::string&& message
    );
    void add_validation_error(ValidationError&& error);
    const std::vector<ParsingError>& get_parsing_errors() const noexcept;
    const std::vector<ValidationError>& get_validation_errors() const noexcept;
    void merge(Errors&& other);
    Errors& operator+=(Errors&& other);
private:
    std::vector<ParsingError> parsing_errors;
    std::vector<ValidationError> validation_errors;
};

} // namespace dnd

#endif // ERRORS_HPP_
