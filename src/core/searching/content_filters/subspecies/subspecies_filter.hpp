#ifndef CHARACTER_SUBRACE_FILTER_HPP_
#define CHARACTER_SUBRACE_FILTER_HPP_

#include <dnd_config.hpp>

#include <core/models/subspecies/subspecies.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

namespace dnd {

class SubspeciesFilter : public ContentPieceFilter {
public:
    bool has_all_filters() const noexcept override;
    bool matches(const Subspecies& subspecies) const noexcept;
    std::vector<const ContentPiece*> all_matches(const Content& content) const override;
    void clear() override;
};

} // namespace dnd

#endif // CHARACTER_SUBRACE_FILTER_HPP_
