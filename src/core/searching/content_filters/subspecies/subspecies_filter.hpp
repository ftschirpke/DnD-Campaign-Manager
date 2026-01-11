#ifndef SUBSPECIES_FILTER_HPP_
#define SUBSPECIES_FILTER_HPP_

#include <dnd_config.hpp>

#include <vector>

#include <core/models/subspecies/subspecies.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

namespace dnd {

class SubspeciesFilter : public ContentPieceFilter {
public:
    explicit SubspeciesFilter(const Content& content) noexcept;

    bool has_all_filters() const override;
    bool matches(const Subspecies& subspecies) const;
    std::vector<Id> all_matches() const override;
    void clear() override;
};

} // namespace dnd

#endif // SUBSPECIES_FILTER_HPP_
