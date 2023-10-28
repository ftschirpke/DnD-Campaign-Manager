#include <dnd_config.hpp>

#include "content_piece_filter.hpp"

#include <string>
#include <vector>

#include <core/models/content_piece.hpp>
#include <core/searching/content_filters/bool_filtering.hpp>
#include <core/searching/content_filters/string_filtering.hpp>

dnd::ContentPieceFilter::ContentPieceFilter() noexcept
    : ContentFilter(), name_filter_type(StringFilterType::NONE), name_filter(),
      description_filter_type(StringFilterType::NONE), description_filter(),
      is_sourcebook_filter_type(BoolFilterType::NONE) {}

void dnd::ContentPieceFilter::set_name_filter(StringFilterType type, const std::string& name) noexcept {
    remove_name_selection_filter();
    name_filter_type = type;
    name_filter = name;
}

void dnd::ContentPieceFilter::remove_name_filter() noexcept { name_filter_type = StringFilterType::NONE; }

void dnd::ContentPieceFilter::set_name_selection_filter(
    SelectionFilterType type, const std::vector<std::string>& names
) noexcept {
    remove_name_filter();
    name_selection_filter_type = type;
    name_selection_filter = names;
}

void dnd::ContentPieceFilter::remove_name_selection_filter() noexcept {
    name_selection_filter_type = SelectionFilterType::NONE;
}

void dnd::ContentPieceFilter::set_description_filter(StringFilterType type, const std::string& description) noexcept {
    description_filter_type = type;
    description_filter = description;
}

void dnd::ContentPieceFilter::remove_description_filter() noexcept { description_filter_type = StringFilterType::NONE; }

void dnd::ContentPieceFilter::set_is_sourcebook_filter(BoolFilterType type) noexcept {
    is_sourcebook_filter_type = type;
}

void dnd::ContentPieceFilter::remove_is_sourcebook_filter() noexcept {
    is_sourcebook_filter_type = BoolFilterType::NONE;
}

bool dnd::ContentPieceFilter::matches(const dnd::ContentPiece& content_piece) const noexcept {
    return matches_string(content_piece.get_name(), name_filter_type, name_filter)
           && matches_selection(content_piece.get_name(), name_selection_filter_type, name_selection_filter)
           && matches_string(content_piece.get_description(), description_filter_type, description_filter)
           && matches_bool(content_piece.get_source_info().is_from_source_book(), is_sourcebook_filter_type);
}
