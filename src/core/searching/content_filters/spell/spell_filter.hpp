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
    /**
     * @brief Determines whether all filters are set
     * @return "true" if all filters are set, "false" otherwise
     */
    bool has_all_filters() const noexcept override;

    /**
     * @brief Determines if a spell matches the filter
     * @param content the spell to check
     * @return "true" if the spell matches the filter, "false" otherwise
     */
    bool matches(const Spell& spell) const noexcept;

    /**
     * @brief Get all content pieces that match the filter
     * @param content the content to search through
     * @return a vector of pointers to the content pieces that match the filter
     */
    std::vector<const ContentPiece*> all_matches(const Content& content) const override;

    /**
     * @brief Clears all filter settings
     */
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
