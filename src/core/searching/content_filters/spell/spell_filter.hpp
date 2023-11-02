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
    BoolFilter& get_verbal_component_filter() noexcept;
    BoolFilter& get_somatic_component_filter() noexcept;
    BoolFilter& get_material_component_filter() noexcept;
    NumberFilter<int>& get_level_filter() noexcept;
    SelectionFilter<MagicSchool>& get_magic_school_filter() noexcept;
    BoolFilter& get_ritual_filter() noexcept;
    StringFilter& get_casting_time_filter() noexcept;
    StringFilter& get_range_filter() noexcept;
    StringFilter& get_duration_filter() noexcept;
    SelectionFilter<std::string>& get_classes_filter() noexcept;

    /**
     * @brief Determines if a spell matches the filter
     * @param content the spell to check
     * @return "true" if the spell matches the filter, "false" otherwise
     */
    bool matches(const Spell& spell) const noexcept;

    /**
     * @brief Accepts a visitor to perform an operation on the filter.
     * @param visitor reference to the visitor
     */
    void accept(ContentFilterVisitor& visitor) override;
private:
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
