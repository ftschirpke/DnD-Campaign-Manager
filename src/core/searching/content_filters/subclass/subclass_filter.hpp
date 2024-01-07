#ifndef SUBCLASS_FILTER_HPP_
#define SUBCLASS_FILTER_HPP_

#include <dnd_config.hpp>

#include <vector>

#include <core/models/subclass/subclass.hpp>
#include <core/searching/content_filters/bool_filter.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

namespace dnd {

class SubclassFilter : public ContentPieceFilter {
public:
    bool has_all_filters() const noexcept override;
    bool matches(const Subclass& subclass) const noexcept;
    std::vector<const ContentPiece*> all_matches(const Content& content) const override;
    void clear() override;

    BoolFilter has_spellcasting_filter;
};

} // namespace dnd

#endif // SUBCLASS_FILTER_HPP_
