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
#include <core/visitors/filters/content_filter_visitor.hpp>


namespace dnd {

using NameFilterVariant = std::variant<StringFilter, SelectionFilter<std::string>>;

class ContentPiece;

class ContentPieceFilter : public ContentFilter {
public:
    ContentPieceFilter() noexcept;

    NameFilterVariant& get_name_filter() noexcept;
    StringFilter& get_description_filter() noexcept;
    BoolFilter& get_is_sourcebook_filter() noexcept;

    /**
     * @brief Determines whether a content piece matches the filter
     * @param content the content piece to check
     * @return "true" if the content piece matches the filter, "false" otherwise
     */
    bool matches(const ContentPiece& content_piece) const noexcept;

    /**
     * @brief Accepts a visitor to perform an operation on the filter.
     * @param visitor reference to the visitor
     */
    void accept(ContentFilterVisitor& visitor) override;
private:
    NameFilterVariant name_filter;
    StringFilter description_filter;
    BoolFilter is_sourcebook_filter;
};

} // namespace dnd

#endif // CONTENT_PIECE_FILTER_HPP_
