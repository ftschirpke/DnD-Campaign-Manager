#include <dnd_config.hpp>

#include "content_piece.hpp"

#include <filesystem>
#include <string>
#include <utility>

#include <core/content_visitors/content_visitor.hpp>

dnd::ContentPiece::ContentPiece(
    const std::string& name, const std::string& description, const std::filesystem::path& source_path
) noexcept
    : name(name), description(description), source_path(source_path) {}

dnd::ContentPiece::ContentPiece(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path
) noexcept
    : name(std::move(name)), description(std::move(description)), source_path(std::move(source_path)) {}

const std::string& dnd::ContentPiece::get_name() const { return name; }

const std::string& dnd::ContentPiece::get_description() const { return description; }

const std::filesystem::path& dnd::ContentPiece::get_source_path() const { return source_path; }
