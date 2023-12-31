#ifndef ABILITY_SCORES_HPP_
#define ABILITY_SCORES_HPP_

#include <dnd_config.hpp>

#include <core/basic_mechanics/abilities.hpp>
#include <core/utils/data_result.hpp>
#include <core/validation/character/ability_scores_data.hpp>

namespace dnd {

class AbilityScores {
public:
    using Data = AbilityScoresData;

    static CreateResult<AbilityScores> create(Data&& data);

    int get(Ability ability) const noexcept;
    int get_strength() const noexcept;
    int get_dexterity() const noexcept;
    int get_constitution() const noexcept;
    int get_intelligence() const noexcept;
    int get_wisdom() const noexcept;
    int get_charisma() const noexcept;

    int get_modifier(Ability ability) const noexcept;
    int get_strength_modifier() const noexcept;
    int get_dexterity_modifier() const noexcept;
    int get_constitution_modifier() const noexcept;
    int get_intelligence_modifier() const noexcept;
    int get_wisdom_modifier() const noexcept;
    int get_charisma_modifier() const noexcept;
private:
    AbilityScores(int strength, int dexterity, int constitution, int intelligence, int wisdom, int charisma) noexcept;

    int strength;
    int dexterity;
    int constitution;
    int intelligence;
    int wisdom;
    int charisma;
};

} // namespace dnd

#endif // ABILITY_SCORES_HPP_
