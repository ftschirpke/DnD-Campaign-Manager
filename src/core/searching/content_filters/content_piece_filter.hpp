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

    bool has_name_filter() const noexcept;
    bool has_description_filter() const noexcept;
    bool has_is_sourcebook_filter() const noexcept;
    bool has_all_filters() const noexcept;

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
     * @brief Get all content pieces that match the filter
     * @param content the content to search through
     * @return a vector of pointers to the content pieces that match the filter
     */
    std::vector<const ContentPiece*> all_matches(const Content& content) const override;

    /**
     * @brief Clears all filter settings
     */
    void clear() override;
private:
    NameFilterVariant name_filter;
    StringFilter description_filter;
    BoolFilter is_sourcebook_filter;
};

} // namespace dnd

#endif // CONTENT_PIECE_FILTER_HPP_
