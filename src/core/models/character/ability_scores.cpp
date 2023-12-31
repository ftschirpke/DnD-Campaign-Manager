#include <dnd_config.hpp>

#include "ability_scores.hpp"

#include <core/basic_mechanics/abilities.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/validation/character/ability_scores_data.hpp>

namespace dnd {

static int calculate_modifier(int score) noexcept { return score / 2 - 5; }

CreateResult<AbilityScores> AbilityScores::create(Data&& data) {
    Errors errors = data.validate();
    if (!errors.ok()) {
        return InvalidCreate<AbilityScores>(std::move(data), std::move(errors));
    }
    return ValidCreate(AbilityScores(
        data.ability_scores[0], data.ability_scores[1], data.ability_scores[2], data.ability_scores[3],
        data.ability_scores[4], data.ability_scores[5]
    ));
}

int AbilityScores::get(Ability ability) const noexcept {
    switch (ability) {
        case Ability::STRENGTH:
            return strength;
        case Ability::DEXTERITY:
            return dexterity;
        case Ability::CONSTITUTION:
            return constitution;
        case Ability::INTELLIGENCE:
            return intelligence;
        case Ability::WISDOM:
            return wisdom;
        case Ability::CHARISMA:
            return charisma;
        default:
            return 0;
    }
}

int AbilityScores::get_strength() const noexcept { return strength; }

int AbilityScores::get_dexterity() const noexcept { return dexterity; }

int AbilityScores::get_constitution() const noexcept { return constitution; }

int AbilityScores::get_intelligence() const noexcept { return intelligence; }

int AbilityScores::get_wisdom() const noexcept { return wisdom; }

int AbilityScores::get_charisma() const noexcept { return charisma; }

int AbilityScores::get_modifier(Ability ability) const noexcept {
    switch (ability) {
        case Ability::STRENGTH:
            return get_strength_modifier();
        case Ability::DEXTERITY:
            return get_dexterity_modifier();
        case Ability::CONSTITUTION:
            return get_constitution_modifier();
        case Ability::INTELLIGENCE:
            return get_intelligence_modifier();
        case Ability::WISDOM:
            return get_wisdom_modifier();
        case Ability::CHARISMA:
            return get_charisma_modifier();
        default:
            return 0;
    }
}

int AbilityScores::get_strength_modifier() const noexcept { return calculate_modifier(strength); }

int AbilityScores::get_dexterity_modifier() const noexcept { return calculate_modifier(dexterity); }

int AbilityScores::get_constitution_modifier() const noexcept { return calculate_modifier(constitution); }

int AbilityScores::get_intelligence_modifier() const noexcept { return calculate_modifier(intelligence); }

int AbilityScores::get_wisdom_modifier() const noexcept { return calculate_modifier(wisdom); }

int AbilityScores::get_charisma_modifier() const noexcept { return calculate_modifier(charisma); }


AbilityScores::AbilityScores(
    int strength, int dexterity, int constitution, int intelligence, int wisdom, int charisma
) noexcept
    : strength(strength), dexterity(dexterity), constitution(constitution), intelligence(intelligence), wisdom(wisdom),
      charisma(charisma) {}

} // namespace dnd
