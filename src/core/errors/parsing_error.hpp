#ifndef PARSING_ERROR_HPP_
#define PARSING_ERROR_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <string>

namespace dnd {

class ParsingError {
public:
    enum class Code;

    ParsingError(Code error_code, const std::filesystem::path& filepath, const std::string& message);

    Code get_error_code() const;
    const std::filesystem::path& get_filepath() const;
    const std::string& get_error_message() const;
private:
    Code error_code;
    std::filesystem::path filepath;
    std::string error_message;
};

enum class ParsingError::Code {
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

} // namespace dnd

#endif // PARSING_ERROR_HPP_
