#include <dnd_config.hpp>

#include "parsing_error.hpp"

#include <filesystem>
#include <string>

namespace dnd {

ParsingError::ParsingError(
    ParsingError::Code error_code, const std::filesystem::path& filepath, const std::string& message
) noexcept
    : error_code(error_code), filepath(filepath), error_message(message) {}

ParsingError::Code ParsingError::get_error_code() const noexcept { return error_code; }

const std::filesystem::path& ParsingError::get_filepath() const noexcept { return filepath; }

const std::string& ParsingError::get_error_message() const noexcept { return error_message; }

} // namespace dnd
