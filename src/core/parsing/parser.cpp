#include <dnd_config.hpp>

#include "parser.hpp"

#include <filesystem>

namespace dnd {

const std::filesystem::path& Parser::get_filepath() const { return filepath; }

Parser::Parser(const std::filesystem::path& filepath) : filepath(filepath) {}

} // namespace dnd
