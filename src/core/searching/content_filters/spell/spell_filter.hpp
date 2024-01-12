#ifndef SPELL_FILTER_HPP_
#define SPELL_FILTER_HPP_

#include <dnd_config.hpp>

#include <string>
#include <vector>

#include <core/models/spell/spell_components.hpp>
#include <core/models/spell/spell_type.hpp>
#include <core/searching/content_filters/bool_filter.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>
#include <core/searching/content_filters/number_filter.hpp>
#include <core/searching/content_filters/selection_filter.hpp>

namespace dnd {

class Spell;

class SpellFilter : public ContentPieceFilter {
public:
    bool has_all_filters() const override;
    bool matches(const Spell& spell) const;
    std::vector<const ContentPiece*> all_matches(const Content& content) const override;
    void clear() override;

    BoolFilter verbal_component_filter;
    BoolFilter somatic_component_filter;
    BoolFilter material_component_filter;
    NumberFilter<int> level_filter;
    SelectionFilter<MagicSchool> magic_school_filter;
    BoolFilter ritual_filter;
    StringFilter casting_time_filter;
    StringFilter range_filter;
    StringFilter duration_filter;
    SelectionFilter<std::string> classes_filter;
};

} // namespace dnd

#endif // SPELL_FILTER_HPP_
