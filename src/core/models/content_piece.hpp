#ifndef CONTENT_PIECE_HPP_
#define CONTENT_PIECE_HPP_

#include <dnd_config.hpp>

#include <string>

#include <fmt/format.h>

#include <core/models/source_info.hpp>
#include <core/text/text.hpp>

namespace dnd {

class ContentVisitor;

/**
 * @brief Interface for content pieces i.e. everything that can be displayed in the application.
 */
class ContentPiece {
public:
    virtual ~ContentPiece() = default;
    virtual const std::string& get_name() const = 0;
    virtual const Text& get_description() const = 0;
    virtual const SourceInfo& get_source_info() const = 0;
    virtual const std::string& get_key() const = 0;
    virtual void accept_visitor(ContentVisitor& visitor) const = 0;
};

template <typename T>
concept isContentPieceType = std::is_base_of_v<ContentPiece, T>;

} // namespace dnd

#endif // CONTENT_PIECE_HPP_
