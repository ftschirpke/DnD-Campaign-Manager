#ifndef CONTENT_PIECE_FILTER_HPP_
#define CONTENT_PIECE_FILTER_HPP_

#include <dnd_config.hpp>

#include <string>
#include <variant>
#include <vector>

#include <core/searching/content_filters/bool_filter.hpp>
#include <core/searching/content_filters/content_filter.hpp>
#include <core/searching/content_filters/selection_filter.hpp>
#include <core/searching/content_filters/string_filter.hpp>

namespace dnd {

using NameFilterVariant = std::variant<StringFilter, SelectionFilter<std::string>>;

class ContentPiece;

class ContentPieceFilter : public ContentFilter {
public:
    ContentPieceFilter();
    bool has_all_filters() const override;
    bool matches(const ContentPiece& content_piece) const;
    std::vector<const ContentPiece*> all_matches(const Content& content) const override;
    void clear() override;

    NameFilterVariant name_filter;
    // StringFilter description_filter; // TODO: replace with a TextFilter
    BoolFilter is_sourcebook_filter;
};

} // namespace dnd

#endif // CONTENT_PIECE_FILTER_HPP_
