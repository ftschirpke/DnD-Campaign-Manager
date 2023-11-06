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
    bool has_verbal_component_filter() const noexcept;
    bool has_somatic_component_filter() const noexcept;
    bool has_material_component_filter() const noexcept;
    bool has_level_filter() const noexcept;
    bool has_magic_school_filter() const noexcept;
    bool has_ritual_filter() const noexcept;
    bool has_casting_time_filter() const noexcept;
    bool has_range_filter() const noexcept;
    bool has_duration_filter() const noexcept;
    bool has_classes_filter() const noexcept;

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
     * @brief Get all content pieces that match the filter
     * @param content the content to search through
     * @return a vector of pointers to the content pieces that match the filter
     */
    std::vector<const ContentPiece*> all_matches(const Content& content) const override;

    /**
     * @brief Clears all filter settings
     */
    void clear() override;
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
