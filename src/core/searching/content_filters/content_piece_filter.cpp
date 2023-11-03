#include <dnd_config.hpp>

#include "content_piece_filter.hpp"

#include <string>
#include <variant>
#include <vector>

#include <core/models/content_piece.hpp>
#include <core/searching/content_filters/bool_filter.hpp>
#include <core/searching/content_filters/string_filter.hpp>
#include <core/visitors/filters/content_filter_visitor.hpp>

dnd::ContentPieceFilter::ContentPieceFilter() noexcept
    : name_filter(StringFilter()), description_filter(), is_sourcebook_filter() {}

bool dnd::ContentPieceFilter::has_name_filter() const noexcept {
    switch (name_filter.index()) {
        case 0:
            return std::get<0>(name_filter).get_type() != StringFilterType::NONE;
        case 1:
            return std::get<1>(name_filter).get_type() != SelectionFilterType::NONE;
        default:
            return false;
    }
}

bool dnd::ContentPieceFilter::has_description_filter() const noexcept {
    return description_filter.get_type() != StringFilterType::NONE;
}

bool dnd::ContentPieceFilter::has_is_sourcebook_filter() const noexcept {
    return is_sourcebook_filter.get_type() != BoolFilterType::NONE;
}

bool dnd::ContentPieceFilter::has_all_filters() const noexcept {
    return has_name_filter() && has_description_filter() && has_is_sourcebook_filter();
}

dnd::NameFilterVariant& dnd::ContentPieceFilter::get_name_filter() noexcept { return name_filter; }

dnd::StringFilter& dnd::ContentPieceFilter::get_description_filter() noexcept { return description_filter; }

dnd::BoolFilter& dnd::ContentPieceFilter::get_is_sourcebook_filter() noexcept { return is_sourcebook_filter; }

bool dnd::ContentPieceFilter::matches(const ContentPiece& content_piece) const noexcept {
    return std::visit(
               [&content_piece](const auto& filter) { return filter.matches(content_piece.get_name()); }, name_filter
           )
           && description_filter.matches(content_piece.get_description())
           && is_sourcebook_filter.matches(content_piece.get_source_info().is_from_source_book());
}

void dnd::ContentPieceFilter::accept(dnd::ContentFilterVisitor& visitor) { visitor.visit(*this); }
