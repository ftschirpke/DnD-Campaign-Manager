#ifndef ABILITY_SCORES_HPP_
#define ABILITY_SCORES_HPP_

#include <dnd_config.hpp>

#include <array>
#include <compare>

#include <core/basic_mechanics/abilities.hpp>
#include <core/utils/data_result.hpp>

namespace dnd {

class AbilityScores {
public:
    class Data;

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

class AbilityScores::Data {
public:
    std::strong_ordering operator<=>(const Data&) const noexcept = default;

    std::array<int, 6> ability_scores;
};

} // namespace dnd

#endif // ABILITY_SCORES_HPP_
