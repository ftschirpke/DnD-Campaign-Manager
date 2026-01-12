#include <dnd_config.hpp>

#include "choosable_filter.hpp"

#include <vector>

#include <core/content.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

namespace dnd {

ChoosableFilter::ChoosableFilter(const Content& content) noexcept : ContentPieceFilter(content) {}

bool ChoosableFilter::has_all_filters() const {
    return ContentPieceFilter::has_all_filters() && type_filter.is_set() && has_prerequisites_filter.is_set();
}

bool ChoosableFilter::matches(const Choosable& choosable) const {
    return ContentPieceFilter::matches(choosable) && type_filter.matches(choosable.get_type())
           && has_prerequisites_filter.matches(!choosable.get_prerequisites().empty());
}

std::vector<Id> ChoosableFilter::all_matches() const {
    std::vector<Id> matching_content_pieces;
    const std::vector<Choosable>& choosables = content.get().get_all_choosables();
    for (size_t i = 0; i < choosables.size(); ++i) {
        if (matches(choosables[i])) {
            matching_content_pieces.push_back(Id{.index = i, .type = Type::Choosable});
        }
    }
    return matching_content_pieces;
}

void ChoosableFilter::clear() {
    ContentPieceFilter::clear();
    type_filter.clear();
    has_prerequisites_filter.clear();
}

} // namespace dnd
