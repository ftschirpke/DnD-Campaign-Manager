#ifndef CONTENT_PIECE_HPP_
#define CONTENT_PIECE_HPP_

#include "dnd_config.hpp"

#include <filesystem>
#include <string>

namespace dnd {

class ContentPiece {
public:
    ContentPiece(const std::string& name, const std::filesystem::path& source_file_path);
    const std::string name;
    const std::filesystem::path source_file_path;
};

inline ContentPiece::ContentPiece(const std::string& name, const std::filesystem::path& source_file_path)
    : name(name), source_file_path(source_file_path) {}

} // namespace dnd

#endif // CONTENT_PIECE_HPP_
