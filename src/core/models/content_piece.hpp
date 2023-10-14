#ifndef CONTENT_PIECE_HPP_
#define CONTENT_PIECE_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <string>

#include <core/models/source_info.hpp>

namespace dnd {

class ContentVisitor;

/**
 * @brief Interface for content pieces i.e. everything that can be displayed in the application.
 */
class ContentPiece {
public:
    virtual ~ContentPiece() = default;
    /**
     * @brief Returns the name of the content piece
     * @return name of the content piece
     */
    virtual const std::string& get_name() const = 0;
    /**
     * @brief Returns the description of the content piece
     * @return description of the content piece
     */
    virtual const std::string& get_description() const = 0;
    /**
     * @brief Returns the information about the source of the content piece
     * @return the information about the source of the content piece
     */
    virtual const SourceInfo& get_source_info() const = 0;
    /**
     * @brief Accepts a visitor
     * @param visitor reference to the visitor
     */
    virtual void accept(ContentVisitor& visitor) const = 0;
};

template <typename T>
concept isContentPieceType = std::is_base_of_v<ContentPiece, T>;

} // namespace dnd

#endif // CONTENT_PIECE_HPP_
