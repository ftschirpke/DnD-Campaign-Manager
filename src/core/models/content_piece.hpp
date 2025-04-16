#ifndef CONTENT_PIECE_HPP_
#define CONTENT_PIECE_HPP_

#include <dnd_config.hpp>

#include <string>

#include <fmt/format.h>

#include <core/content_keys.hpp>
#include <core/models/source_info.hpp>

namespace dnd {

class ContentVisitor;

/**
 * @brief Interface for content pieces i.e. everything that can be displayed in the application.
 */
class ContentPiece {
public:
    virtual ~ContentPiece() = default;
    virtual const std::string& get_name() const = 0;
    virtual const std::string& get_description() const = 0;
    virtual const SourceInfo& get_source_info() const = 0;
    virtual std::string get_key() const;
    virtual void accept_visitor(ContentVisitor& visitor) const = 0;
};

inline std::string ContentPiece::get_key() const { return default_key(get_name(), get_source_info().name); }

template <typename T>
concept isContentPieceType = std::is_base_of_v<ContentPiece, T>;

} // namespace dnd

#endif // CONTENT_PIECE_HPP_
