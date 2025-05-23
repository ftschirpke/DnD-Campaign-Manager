#include <dnd_config.hpp>

#include "content_piece_filter.hpp"

#include <variant>
#include <vector>

#include <core/content.hpp>
#include <core/models/content_piece.hpp>
#include <core/searching/content_filters/bool_filter.hpp>
#include <core/searching/content_filters/string_filter.hpp>

namespace dnd {

ContentPieceFilter::ContentPieceFilter() : name_filter(StringFilter()), is_sourcebook_filter() {}

bool ContentPieceFilter::has_all_filters() const {
    return std::visit([](const auto& filter) { return filter.is_set(); }, name_filter) && is_sourcebook_filter.is_set();
}

bool ContentPieceFilter::matches(const ContentPiece& content_piece) const {
    return std::visit(
        [&content_piece](const auto& filter) { return filter.matches(content_piece.get_name()); }, name_filter
    );
}

std::vector<const ContentPiece*> ContentPieceFilter::all_matches(const Content& content) const {
    std::vector<const ContentPiece*> matching_content_pieces;
    for (const auto& [_, character] : content.get_characters().get_all()) {
        if (matches(character)) {
            matching_content_pieces.push_back(&character);
        }
    }
    for (const auto& [_, cls] : content.get_classes().get_all()) {
        if (matches(cls)) {
            matching_content_pieces.push_back(&cls);
        }
    }
    for (const auto& [_, subclass] : content.get_subclasses().get_all()) {
        if (matches(subclass)) {
            matching_content_pieces.push_back(&subclass);
        }
    }
    for (const auto& [_, species] : content.get_species().get_all()) {
        if (matches(species)) {
            matching_content_pieces.push_back(&species);
        }
    }
    for (const auto& [_, subspecies] : content.get_subspecies().get_all()) {
        if (matches(subspecies)) {
            matching_content_pieces.push_back(&subspecies);
        }
    }
    for (const auto& [_, item] : content.get_items().get_all()) {
        if (matches(item)) {
            matching_content_pieces.push_back(&item);
        }
    }
    for (const auto& [_, spell] : content.get_spells().get_all()) {
        if (matches(spell)) {
            matching_content_pieces.push_back(&spell);
        }
    }
    for (const auto& [_, feature] : content.get_features().get_all()) {
        if (matches(feature)) {
            matching_content_pieces.push_back(&feature.get());
        }
    }
    for (const auto& [_, class_feature] : content.get_class_features().get_all()) {
        if (matches(class_feature)) {
            matching_content_pieces.push_back(&class_feature.get());
        }
    }
    for (const auto& [_, choosable] : content.get_choosables().get_all()) {
        if (matches(choosable)) {
            matching_content_pieces.push_back(&choosable);
        }
    }
    return matching_content_pieces;
}

void ContentPieceFilter::clear() {
    name_filter = StringFilter();
    is_sourcebook_filter.clear();
}

} // namespace dnd
