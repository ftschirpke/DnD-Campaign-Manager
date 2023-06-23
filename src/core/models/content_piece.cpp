#include <dnd_config.hpp>

#include "content_piece.hpp"

#include <filesystem>
#include <string>
#include <utility>

#include <core/models/source_info.hpp>

dnd::ContentPiece::ContentPiece(
    const std::string& name, const std::string& description, const std::filesystem::path& source_path
) noexcept
    : name(name), description(description), source_info(source_path) {}

dnd::ContentPiece::ContentPiece(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path
) noexcept
    : name(std::move(name)), description(std::move(description)), source_info(std::move(source_path)) {}

const std::string& dnd::ContentPiece::get_name() const { return name; }

const std::string& dnd::ContentPiece::get_description() const { return description; }

const dnd::SourceInfo& dnd::ContentPiece::get_source_info() const { return source_info; }
