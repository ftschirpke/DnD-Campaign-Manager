#ifndef CONTENT_PIECE_FILTER_HPP_
#define CONTENT_PIECE_FILTER_HPP_

#include <dnd_config.hpp>

#include <string>
#include <vector>

#include <core/searching/content_filters/bool_filtering.hpp>
#include <core/searching/content_filters/content_filter.hpp>
#include <core/searching/content_filters/selection_filtering.hpp>
#include <core/searching/content_filters/string_filtering.hpp>

namespace dnd {

class ContentPiece;

class ContentPieceFilter : public ContentFilter {
public:
    ContentPieceFilter() noexcept;

    /**
     * @brief Set a string filter for the name of content pieces
     * @param type the type of filter to apply
     * @param name the name to filter by
     * @details Removes filters vector by set_name_selection_filter
     */
    void set_name_filter(StringFilterType type, const std::string& name) noexcept;
    void remove_name_filter() noexcept;
    /**
     * @brief Set a selection filter for the name of content pieces
     * @param type the type of filter to apply
     * @param names the names to filter by
     * @details Removes filters vector by set_name_filter
     */
    void set_name_selection_filter(SelectionFilterType type, const std::vector<std::string>& names) noexcept;
    void remove_name_selection_filter() noexcept;

    void set_description_filter(StringFilterType type, const std::string& description) noexcept;
    void remove_description_filter() noexcept;
    void set_is_sourcebook_filter(BoolFilterType type) noexcept;
    void remove_is_sourcebook_filter() noexcept;

    /**
     * @brief Determines whether a content piece matches the filter
     * @param content the content piece to check
     * @return "true" if the content piece matches the filter, "false" otherwise
     */
    bool matches(const ContentPiece& content_piece) const noexcept;
private:
    StringFilterType name_filter_type;
    std::string name_filter;
    SelectionFilterType name_selection_filter_type;
    std::vector<std::string> name_selection_filter;
    StringFilterType description_filter_type;
    std::string description_filter;
    BoolFilterType is_sourcebook_filter_type;
};

} // namespace dnd

#endif // CONTENT_PIECE_FILTER_HPP_
