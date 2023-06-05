#ifndef CONTENT_PIECE_HPP_
#define CONTENT_PIECE_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <string>

#include <core/content_visitors/content_visitor.hpp>

namespace dnd {

class ContentPiece {
public:
    ContentPiece(const std::string& name, const std::filesystem::path& source_file_path);
    virtual ~ContentPiece() = default;
    /**
     * @brief Accepts a visitor
     * @param visitor pointer to the visitor
     */
    virtual void accept(ContentVisitor* visitor) const = 0;

    const std::string name;
    const std::filesystem::path source_file_path;
};

template <typename T>
concept ContentPieceType = std::is_base_of_v<ContentPiece, T>;


inline ContentPiece::ContentPiece(const std::string& name, const std::filesystem::path& source_file_path)
    : name(name), source_file_path(source_file_path) {}

} // namespace dnd

#endif // CONTENT_PIECE_HPP_
