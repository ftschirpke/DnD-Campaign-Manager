#ifndef EFFECT_HOLDER_HPP_
#define EFFECT_HOLDER_HPP_

#include <dnd_config.hpp>

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <core/models/effect_holder/condition/condition.hpp>
#include <core/models/effect_holder/effect/effect.hpp>
#include <core/models/effect_holder/subholders/action_holder.hpp>
#include <core/models/effect_holder/subholders/extra_spells_holder.hpp>
#include <core/models/effect_holder/subholders/proficiency_holder.hpp>
#include <core/models/effect_holder/subholders/riv_holder.hpp>
#include <core/validation/effect_holder/effect_holder_data.hpp>

namespace dnd {

/**
 * @brief A class representing a part of a feature or objects alike that provides certain effects to a character
 */
class EffectHolder {
public:
    /**
     * @brief Constructs an effect holder from the given data
     * @param data the data to construct the effect holder from
     * @return the constructed effect holder
     * @throws dnd::invalid_data if the data is invalid
     */
    static EffectHolder create(EffectHolderData&& data, const ContentHolder* content);

    /**
     * @brief Constructs an effect holder
     * @param activation_conditions the conditions that need to be met for the effects to be activated
     * @param effects the effects that are activated when the conditions are met
     * @param action_holder the actions provided by the effect holder
     * @param extra_spells_holder the extra spells provided by the effect holder
     * @param proficiency_holder the proficiencies provided by the effect holder
     * @param riv_holder the resistances, immunities, and vulnerabilities provided by the effect holder
     */
    EffectHolder(
        std::vector<std::unique_ptr<Condition>>&& activation_conditions, std::vector<std::unique_ptr<Effect>>&& effects,
        ActionHolder&& action_holder, ExtraSpellsHolder&& extra_spells_holder, ProficiencyHolder&& proficiency_holder,
        RIVHolder&& riv_holder
    ) noexcept;
    virtual ~EffectHolder() noexcept = default;

    const std::vector<std::unique_ptr<Condition>>& get_activation_conditions() const;
    const std::vector<std::unique_ptr<Effect>>& get_effects() const;
    const ActionHolder& get_actions() const;
    const ExtraSpellsHolder& get_extra_spells() const;
    const ProficiencyHolder& get_proficiencies() const;
    const RIVHolder& get_rivs() const;

    /**
     * @brief Returns whether the effect holder is empty
     * @return "true" if the effect holder is empty, "false" otherwise
     */
    bool empty() const;
    /**
     * @brief Checks whether the effect holder is active (provides its effects) for given attributes and constants
     * @param attributes the character attributes
     * @param constants the character constants
     * @return "true" if the effect holder is active for a character with these attributes and constants,
     * "false" otherwise
     */
    bool is_active(
        const std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
    ) const;
private:
    std::vector<std::unique_ptr<Condition>> activation_conditions;
    std::vector<std::unique_ptr<Effect>> effects;
    ActionHolder actions;
    ExtraSpellsHolder extra_spells;
    ProficiencyHolder proficiencies;
    RIVHolder rivs;
};

} // namespace dnd

#endif // EFFECT_HOLDER_HPP_
