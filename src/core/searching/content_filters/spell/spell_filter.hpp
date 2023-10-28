#ifndef SPELL_FILTER_HPP_
#define SPELL_FILTER_HPP_

#include <dnd_config.hpp>

#include <string>
#include <vector>

#include <core/models/spell/spell_components.hpp>
#include <core/models/spell/spell_type.hpp>
#include <core/searching/content_filters/bool_filtering.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>
#include <core/searching/content_filters/number_filtering.hpp>
#include <core/searching/content_filters/selection_filtering.hpp>

namespace dnd {

class Spell;

class SpellFilter : public ContentPieceFilter {
public:
    SpellFilter() noexcept;
    /**
     * @brief Accepts a visitor to perform an operation on the filter.
     * @param visitor reference to the visitor
     */
    virtual void accept(ContentFilterVisitor& visitor) override;

    void set_verbal_component_filter(BoolFilterType type) noexcept;
    void remove_verbal_component_filter() noexcept;
    void set_somatic_component_filter(BoolFilterType type) noexcept;
    void remove_somatic_component_filter() noexcept;
    void set_material_component_filter(BoolFilterType type) noexcept;
    void remove_material_component_filter() noexcept;
    void set_level_filter(NumberFilterType type, int level) noexcept;
    void remove_level_filter() noexcept;
    void set_magic_school_filter(SelectionFilterType type, std::vector<MagicSchool> magic_schools) noexcept;
    void remove_magic_school_filter() noexcept;
    void set_ritual_filter(BoolFilterType type) noexcept;
    void remove_ritual_filter() noexcept;
    void set_casting_time_filter(StringFilterType type, const std::string& casting_time) noexcept;
    void remove_casting_time_filter() noexcept;
    void set_range_filter(StringFilterType type, const std::string& range) noexcept;
    void remove_range_filter() noexcept;
    void set_duration_filter(StringFilterType type, const std::string& duration) noexcept;
    void remove_duration_filter() noexcept;
    void set_classes_filter(SelectionFilterType type, std::vector<std::string> classes) noexcept;
    void remove_classes_filter() noexcept;

    /**
     * @brief Determines if a spell matches the filter
     * @param content the spell to check
     * @return "true" if the spell matches the filter, "false" otherwise
     */
    bool matches(const Spell& spell) const noexcept;
private:
    BoolFilterType verbal_component_filter_type;
    BoolFilterType somatic_component_filter_type;
    BoolFilterType material_component_filter_type;
    NumberFilterType level_filter_type;
    int level_filter_level;
    SelectionFilterType magic_school_filter_type;
    std::vector<MagicSchool> magic_school_filter_magic_schools;
    BoolFilterType ritual_filter_type;
    StringFilterType casting_time_filter_type;
    std::string casting_time_filter;
    StringFilterType range_filter_type;
    std::string range_filter;
    StringFilterType duration_filter_type;
    std::string duration_filter;
    SelectionFilterType classes_filter_type;
    std::vector<std::string> classes_filter_classes;
};

} // namespace dnd

#endif // SPELL_FILTER_HPP_
