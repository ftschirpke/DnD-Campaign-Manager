#include <dnd_config.hpp>

#include "content_piece_filter.hpp"

#include <string>
#include <variant>
#include <vector>

#include <core/content.hpp>
#include <core/models/content_piece.hpp>
#include <core/searching/content_filters/bool_filter.hpp>
#include <core/searching/content_filters/string_filter.hpp>
#include <core/visitors/filters/content_filter_visitor.hpp>

dnd::ContentPieceFilter::ContentPieceFilter() noexcept
    : name_filter(StringFilter()), description_filter(), is_sourcebook_filter() {}

bool dnd::ContentPieceFilter::has_name_filter() const noexcept {
    switch (name_filter.index()) {
        case 0:
            return std::get<0>(name_filter).is_set();
        case 1:
            return std::get<1>(name_filter).is_set();
        default:
            return false;
    }
}

bool dnd::ContentPieceFilter::has_description_filter() const noexcept { return description_filter.is_set(); }

bool dnd::ContentPieceFilter::has_is_sourcebook_filter() const noexcept { return is_sourcebook_filter.is_set(); }

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

std::vector<const dnd::ContentPiece*> dnd::ContentPieceFilter::all_matches(const Content& content) const {
    std::vector<const ContentPiece*> matching_content_pieces;
    for (const auto& [_, character] : content.get_characters().get_all()) {
        if (matches(character)) {
            matching_content_pieces.emplace_back(&character);
        }
    }
    for (const auto& [_, character_class] : content.get_character_classes().get_all()) {
        if (matches(character_class)) {
            matching_content_pieces.emplace_back(&character_class);
        }
    }
    for (const auto& [_, character_subclass] : content.get_character_subclasses().get_all()) {
        if (matches(character_subclass)) {
            matching_content_pieces.emplace_back(&character_subclass);
        }
    }
    for (const auto& [_, character_race] : content.get_character_races().get_all()) {
        if (matches(character_race)) {
            matching_content_pieces.emplace_back(&character_race);
        }
    }
    for (const auto& [_, character_subrace] : content.get_character_subraces().get_all()) {
        if (matches(character_subrace)) {
            matching_content_pieces.emplace_back(&character_subrace);
        }
    }
    for (const auto& [_, item] : content.get_items().get_all()) {
        if (matches(item)) {
            matching_content_pieces.emplace_back(&item);
        }
    }
    for (const auto& [_, spell] : content.get_spells().get_all()) {
        if (matches(spell)) {
            matching_content_pieces.emplace_back(&spell);
        }
    }
    for (const auto& [_, feature] : content.get_features().get_all()) {
        if (matches(*feature)) {
            matching_content_pieces.emplace_back(feature);
        }
    }
    for (const auto& [_, choosable] : content.get_choosables().get_all()) {
        if (matches(choosable)) {
            matching_content_pieces.emplace_back(&choosable);
        }
    }
    return matching_content_pieces;
}

void dnd::ContentPieceFilter::clear() {
    name_filter = StringFilter();
    description_filter.clear();
    is_sourcebook_filter.clear();
}
