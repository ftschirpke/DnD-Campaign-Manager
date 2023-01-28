#ifndef CHARACTER_HPP_
#define CHARACTER_HPP_

#include "dnd_config.hpp"

#include <array>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "models/character_class.hpp"
#include "models/character_race.hpp"
#include "models/character_state.hpp"
#include "models/character_subclass.hpp"
#include "models/character_subrace.hpp"
#include "models/effect_holder/character_decision.hpp"
#include "models/effect_holder/feature.hpp"
#include "models/feature_holder.hpp"

namespace dnd {

const std::map<unsigned int, unsigned int> xp_for_level = {
    {1, 0},       {2, 300},     {3, 900},     {4, 2700},    {5, 6500},    {6, 14000},   {7, 23000},
    {8, 34000},   {9, 48000},   {10, 64000},  {11, 85000},  {12, 100000}, {13, 120000}, {14, 140000},
    {15, 165000}, {16, 195000}, {17, 225000}, {18, 265000}, {19, 305000}, {20, 355000},
};

class Character : public FeatureHolder {
public:
    // TODO: should these pointers be non-const?
    const std::array<unsigned int, 6> base_ability_scores;
    CharacterState state;
    const CharacterClass* class_ptr;
    const CharacterSubclass* subclass_ptr;
    const CharacterRace* race_ptr;
    const CharacterSubrace* subrace_ptr;
    std::vector<CharacterDecision> decisions;
    Character(
        const std::string& name, std::vector<Feature>&& features, const std::array<unsigned int, 6>& base_ability_scores
    ) noexcept;
    Character(
        const std::string& name, std::vector<Feature>&& features,
        const std::array<unsigned int, 6>& base_ability_scores, unsigned int level, unsigned int xp,
        const std::vector<unsigned int>& hit_dice_rolls
    ) noexcept;
    unsigned int getLevel() const noexcept;
    unsigned int getXP() const noexcept;
    // void levelUp();
    // void setLevel(unsigned int new_level);
    // void setXP(unsigned int new_xp);
    // void increaseXP(unsigned int xp_increase);
    // void decreaseXP(unsigned int xp_decrease);
    const std::vector<unsigned int>& getHitDiceRolls() const noexcept;
    static unsigned int levelForXP(unsigned int xp);
    // void addHitDiceRoll(int hit_dice_roll);
    void determineState();
    std::vector<const Feature*> allFeatures() const;
protected:
    virtual const std::unordered_map<std::string, int> getConstants() const;
    virtual const std::unordered_map<std::string, int> getInitialAttributeValues() const;
private:
    unsigned int level, xp;
    std::vector<unsigned int> hit_dice_rolls;
    void updateLevel();
};

inline Character::Character(
    const std::string& name, std::vector<Feature>&& features, const std::array<unsigned int, 6>& base_ability_scores
) noexcept
    : FeatureHolder(name, std::move(features)), base_ability_scores(base_ability_scores), state(decisions),
      subclass_ptr(nullptr), subrace_ptr(nullptr) {}


inline Character::Character(
    const std::string& name, std::vector<Feature>&& features, const std::array<unsigned int, 6>& base_ability_scores,
    unsigned int level, unsigned int xp, const std::vector<unsigned int>& hit_dice_rolls
) noexcept
    : FeatureHolder(name, std::move(features)), base_ability_scores(base_ability_scores), state(decisions),
      subclass_ptr(nullptr), subrace_ptr(nullptr), level(level), xp(xp), hit_dice_rolls(hit_dice_rolls) {}

inline unsigned int Character::getLevel() const noexcept { return level; }

inline unsigned int Character::getXP() const noexcept { return xp; }

inline void Character::updateLevel() { level = levelForXP(xp); }

// inline void Character::levelUp() {
//     if (level == 20) {
//         std::cerr << "Warning: Cannot level up beyond level 20.\n"; // TODO: consider throwing exception instead
//         return;
//     }
//     xp = xp_for_level.at(++level);
// }

// inline void Character::setLevel(unsigned int new_level) {
//     if (new_level < 1 || new_level > 20) {
//         std::cerr << "Warning: Level must be between 1 and 20.\n"; // TODO: consider throwing exception instead
//         return;
//     }
//     level = new_level;
//     xp = xp_for_level.at(level);
// }

// inline void Character::setXP(unsigned int new_xp) {
//     xp = new_xp;
//     updateLevel();
// }

// inline void Character::increaseXP(unsigned int xp_increase) {
//     xp += xp_increase;
//     updateLevel();
// }

// inline void Character::decreaseXP(unsigned int xp_decrease) {
//     if (xp_decrease > xp) {
//         std::cerr << "Warning: Cannot decrease XP to less than 0.\n"; // TODO: consider throwing exception instead
//         return;
//     }
//     xp -= xp_decrease;
//     updateLevel();
// }

inline const std::vector<unsigned int>& Character::getHitDiceRolls() const noexcept { return hit_dice_rolls; }

// inline void Character::addHitDiceRoll(int hit_dice_roll) {
//     hit_dice_rolls.push_back(hit_dice_roll);
//     if (hit_dice_rolls.size() > level) {
//         std::cerr << "Warning: More hit dice rolls than level.\n";
//     }
// }

} // namespace dnd

#endif // CHARACTER_HPP_
