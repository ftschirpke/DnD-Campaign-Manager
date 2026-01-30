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
    explicit SubclassFilter(const Content& content) noexcept;

    bool has_all_filters() const override final;
    bool matches(const Subclass& subclass) const;
    std::vector<Id> all_matches() const override final;
    void clear() override final;

    BoolFilter has_spellcasting_filter;
};

} // namespace dnd

#endif // SUBCLASS_FILTER_HPP_
