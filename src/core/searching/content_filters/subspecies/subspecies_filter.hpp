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

    bool has_all_filters() const override final;
    bool matches(const Subspecies& subspecies) const;
    std::vector<Id> all_matches() const override final;
    void clear() override final;
};

} // namespace dnd

#endif // SUBSPECIES_FILTER_HPP_
