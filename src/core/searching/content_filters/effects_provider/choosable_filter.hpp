#ifndef CHOOSABLE_FILTER_HPP_
#define CHOOSABLE_FILTER_HPP_

#include <dnd_config.hpp>

#include <vector>

#include <core/models/effects_provider/choosable.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

namespace dnd {

class ChoosableFilter : public ContentPieceFilter {
public:
    bool has_all_filters() const noexcept override;
    bool matches(const Choosable& choosable) const noexcept;
    std::vector<const ContentPiece*> all_matches(const Content& content) const override;
    void clear() override;

    StringFilter type_filter;
    BoolFilter has_prerequisites_filter;
};

} // namespace dnd

#endif // CHOOSABLE_FILTER_HPP_
