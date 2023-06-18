#include <dnd_config.hpp>

#include "parser.hpp"

#include <filesystem>

const std::filesystem::path& dnd::Parser::get_filepath() const noexcept { return filepath; }

dnd::Parser::Parser(const std::filesystem::path& filepath) noexcept : filepath(filepath) {}
