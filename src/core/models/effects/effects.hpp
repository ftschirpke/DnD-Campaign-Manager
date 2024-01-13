#ifndef EFFECTS_HPP_
#define EFFECTS_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <memory>
#include <vector>

#include <tl/expected.hpp>

#include <core/models/character/stats.hpp>
#include <core/models/effects/choice/choice.hpp>
#include <core/models/effects/condition/condition.hpp>
#include <core/models/effects/stat_change/stat_change.hpp>
#include <core/models/effects/subholders/action_holder.hpp>
#include <core/models/effects/subholders/extra_spells_holder.hpp>
#include <core/models/effects/subholders/proficiency_holder.hpp>
#include <core/models/effects/subholders/riv_holder.hpp>

namespace dnd {

/**
 * @brief A class representing stat_changes of a feature or objects alike to a character
 */
class Effects {
public:
    struct Data;

    static CreateResult<Effects> create_for(Data&& data, const Content& content);

    Effects(
        std::vector<std::unique_ptr<Condition>>&& activation_conditions, std::vector<Choice>&& choices,
        std::vector<std::unique_ptr<StatChange>>&& stat_changes, ActionHolder&& action_holder,
        ExtraSpellsHolder&& extra_spells_holder, ProficiencyHolder&& proficiency_holder, RIVHolder&& riv_holder
    );
    virtual ~Effects() = default;

    Effects(const Effects&) = delete;
    Effects& operator=(const Effects&) = delete;
    Effects(Effects&&) noexcept = default;
    Effects& operator=(Effects&&) noexcept = default;

    bool empty() const;
    const std::vector<std::unique_ptr<Condition>>& get_activation_conditions() const;
    const std::vector<Choice>& get_choices() const;
    const std::vector<std::unique_ptr<StatChange>>& get_stat_changes() const;
    const ActionHolder& get_actions() const;
    const ExtraSpellsHolder& get_extra_spells() const;
    const ProficiencyHolder& get_proficiencies() const;
    const RIVHolder& get_rivs() const;

    tl::expected<bool, Errors> is_active(const Stats& stats) const;

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

struct Effects::Data {
    std::strong_ordering operator<=>(const Data&) const = default;

    std::vector<Condition::Data> activation_conditions_data;
    std::vector<Choice::Data> choices_data;
    std::vector<StatChange::Data> stat_changes_data;
    ActionHolder::Data action_holder_data;
    ExtraSpellsHolder::Data extra_spells_holder_data;
    ProficiencyHolder::Data proficiency_holder_data;
    RIVHolder::Data riv_holder_data;
};

} // namespace dnd

#endif // EFFECTS_HPP_
