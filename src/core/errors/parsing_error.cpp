#include <dnd_config.hpp>

#include "parsing_error.hpp"

#include <filesystem>
#include <string>

dnd::ParsingError::ParsingError(
    dnd::ParsingErrorCode error_code, const std::filesystem::path& filepath, const std::string& message
) noexcept
    : error_code(error_code), filepath(filepath), error_message(message) {}

dnd::ParsingErrorCode dnd::ParsingError::get_error_code() const noexcept { return error_code; }

const std::filesystem::path& dnd::ParsingError::get_filepath() const noexcept { return filepath; }

const std::string& dnd::ParsingError::get_error_message() const noexcept { return error_message; }
