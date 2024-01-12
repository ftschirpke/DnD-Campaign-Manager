#ifndef CLASS_FILTER_HPP_
#define CLASS_FILTER_HPP_

#include <dnd_config.hpp>

#include <core/models/class/class.hpp>
#include <core/searching/content_filters/bool_filter.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

namespace dnd {

class ClassFilter : public ContentPieceFilter {
public:
    bool has_all_filters() const override;
    bool matches(const Class& cls) const;
    std::vector<const ContentPiece*> all_matches(const Content& content) const override;
    void clear() override;

    BoolFilter has_spellcasting_filter;
};

} // namespace dnd

#endif // CLASS_FILTER_HPP_
