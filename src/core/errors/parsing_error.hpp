#ifndef PARSING_ERROR_HPP_
#define PARSING_ERROR_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <string>

namespace dnd {

/**
 * @brief An enum representing the different types of errors that can occur while parsing a file.
 */
enum class ParsingErrorCode {
    // the file was not found
    FILE_NOT_FOUND,
    // the format of the file was invalid
    INVALID_FILE_FORMAT,
    // a required value was not found
    MISSING_ATTRIBUTE,
    // found a value that was not of the expected type
    INVALID_ATTRIBUTE_TYPE,
    // a value was found but it was not expected
    UNEXPECTED_ATTRIBUTE,
    // an unknown error occurred
    UNKNOWN_ERROR,
};

class ParsingError {
public:
    ParsingError(
        ParsingErrorCode error_code, const std::filesystem::path& filepath, const std::string& message
    ) noexcept;

    ParsingErrorCode get_error_code() const noexcept;
    const std::filesystem::path& get_filepath() const noexcept;
    const std::string& get_error_message() const noexcept;
private:
    ParsingErrorCode error_code;
    std::filesystem::path filepath;
    std::string error_message;
};

} // namespace dnd

#endif // PARSING_ERROR_HPP_