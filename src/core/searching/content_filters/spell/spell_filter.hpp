#ifndef SPELL_FILTER_HPP_
#define SPELL_FILTER_HPP_

#include <dnd_config.hpp>

#include <set>

#include <core/models/spell/spell_components.hpp>
#include <core/models/spell/spell_type.hpp>
#include <core/searching/content_filters/content_piece_filter.hpp>

namespace dnd {

class Spell;

class SpellFilter : public ContentPieceFilter {
public:
    SpellFilter() noexcept;

    void set_verbal_component_filter(BoolFilterType type) noexcept;
    void remove_verbal_component_filter() noexcept;
    void set_somatic_component_filter(BoolFilterType type) noexcept;
    void remove_somatic_component_filter() noexcept;
    void set_material_component_filter(BoolFilterType type) noexcept;
    void remove_material_component_filter() noexcept;
    void set_level_filter(NumberFilterType type, int level) noexcept;
    void remove_level_filter() noexcept;
    void set_magic_school_filter(SelectionFilterType type, std::set<MagicSchool> magic_schools) noexcept;
    void remove_magic_school_filter() noexcept;
    void set_ritual_filter(BoolFilterType type) noexcept;
    void remove_ritual_filter() noexcept;
    void set_classes_filter(SelectionFilterType type, std::set<std::string> classes) noexcept;
    void remove_classes_filter() noexcept;

    /**
     * @brief Determines if a spell matches the filter
     * @param content the spell to check
     * @return "true" if the spell matches the filter, "false" otherwise
     */
    bool matches(const Spell& spell) const noexcept;
    /**
     * @brief Get all the names of all spells that match the filter from a given selection of content
     * @param content the content to filter
     * @return the names of all spells that match the filter
     */
    // std::vector<std::string> get_matching(const Content& content) const override;
    // TODO: decide whether to keep this
private:
    BoolFilterType verbal_component_filter_type;
    BoolFilterType somatic_component_filter_type;
    BoolFilterType material_component_filter_type;
    NumberFilterType level_filter_type;
    int level_filter_level;
    SelectionFilterType magic_school_filter_type;
    std::set<MagicSchool> magic_school_filter_magic_schools;
    BoolFilterType ritual_filter_type;
    SelectionFilterType classes_filter_type;
    std::set<std::string> classes_filter_classes;
};

} // namespace dnd

#endif // SPELL_FILTER_HPP_
