#ifndef EFFECTS_HPP_
#define EFFECTS_HPP_

#include <dnd_config.hpp>

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <core/models/effects/choice/choice.hpp>
#include <core/models/effects/condition/condition.hpp>
#include <core/models/effects/stat_change/stat_change.hpp>
#include <core/models/effects/subholders/action_holder.hpp>
#include <core/models/effects/subholders/extra_spells_holder.hpp>
#include <core/models/effects/subholders/proficiency_holder.hpp>
#include <core/models/effects/subholders/riv_holder.hpp>
#include <core/validation/effects/effects_data.hpp>

namespace dnd {

/**
 * @brief A class representing stat_changes of a feature or objects alike to a character
 */
class Effects {
public:
    /**
     * @brief Constructs an Effects object from the given data
     * @param data the data to construct the Effects object from
     * @param content the content to use for the construction
     * @return the constructed Effects object
     * @throws invalid_data if the data is invalid or is incompatible with the given content
     */
    static Effects create(EffectsData&& data, const Content& content);

    /**
     * @brief Constructs an Effects object
     * @param activation_conditions the conditions that need to be met for the stat_changes to be activated
     * @param choices choices that need to be made/are provided
     * @param stat_changes the stat_changes that are activated when the conditions are met
     * @param action_holder the actions provided
     * @param extra_spells_holder the extra spells provided
     * @param proficiency_holder the proficiencies provided
     * @param riv_holder the resistances, immunities, and vulnerabilities provided
     */
    Effects(
        std::vector<std::unique_ptr<Condition>>&& activation_conditions, std::vector<Choice>&& choices,
        std::vector<std::unique_ptr<StatChange>>&& stat_changes, ActionHolder&& action_holder,
        ExtraSpellsHolder&& extra_spells_holder, ProficiencyHolder&& proficiency_holder, RIVHolder&& riv_holder
    ) noexcept;
    Effects(const Effects&) = delete;
    Effects& operator=(const Effects&) = delete;
    Effects(Effects&&) noexcept = default;
    Effects& operator=(Effects&&) noexcept = default;
    virtual ~Effects() noexcept = default;

    const std::vector<std::unique_ptr<Condition>>& get_activation_conditions() const noexcept;
    const std::vector<Choice>& get_choices() const noexcept;
    const std::vector<std::unique_ptr<StatChange>>& get_stat_changes() const noexcept;
    const ActionHolder& get_actions() const noexcept;
    const ExtraSpellsHolder& get_extra_spells() const noexcept;
    const ProficiencyHolder& get_proficiencies() const noexcept;
    const RIVHolder& get_rivs() const noexcept;

    /**
     * @brief Returns whether the effects are empty
     * @return "true" if the object provides no effect, "false" otherwise
     */
    bool empty() const;
    /**
     * @brief Checks whether the activation conditions are met for given attributes and constants
     * @param attributes the character attributes
     * @param constants the character constants
     * @return "true" if the activation conditions are met for a character with these attributes and constants,
     * "false" otherwise
     */
    bool is_active(
        const std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const;
    /**
     * @brief Merge another Effects object into this one
     * @param other the other Effects object which will be merged into this one
     */
    void merge(Effects&& other);
private:
    std::vector<std::unique_ptr<Condition>> activation_conditions;
    std::vector<Choice> choices;
    std::vector<std::unique_ptr<StatChange>> stat_changes;
    ActionHolder actions;
    ExtraSpellsHolder extra_spells;
    ProficiencyHolder proficiencies;
    RIVHolder rivs;
};

} // namespace dnd

#endif // EFFECTS_HPP_
