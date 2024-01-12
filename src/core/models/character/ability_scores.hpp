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
    struct Data;

    static CreateResult<AbilityScores> create(Data&& data);

    int get(Ability ability) const;
    int get_strength() const;
    int get_dexterity() const;
    int get_constitution() const;
    int get_intelligence() const;
    int get_wisdom() const;
    int get_charisma() const;

    int get_modifier(Ability ability) const;
    int get_strength_modifier() const;
    int get_dexterity_modifier() const;
    int get_constitution_modifier() const;
    int get_intelligence_modifier() const;
    int get_wisdom_modifier() const;
    int get_charisma_modifier() const;
private:
    AbilityScores(int strength, int dexterity, int constitution, int intelligence, int wisdom, int charisma);

    int strength;
    int dexterity;
    int constitution;
    int intelligence;
    int wisdom;
    int charisma;
};

struct AbilityScores::Data {
    std::strong_ordering operator<=>(const Data&) const = default;

    std::array<int, 6> ability_scores;
};

} // namespace dnd

#endif // ABILITY_SCORES_HPP_
