#ifndef CHARACTER_HPP_
#define CHARACTER_HPP_

#include <array>
#include <iostream>
#include <map>
#include <memory>
#include <vector>

#include "models/character_class.hpp"
#include "models/character_race.hpp"
#include "models/creature.hpp"
#include "models/effect_holder/feature.hpp"

namespace dnd {

const std::map<int, int> xp_for_level = {
    {1, 0},       {2, 300},     {3, 900},     {4, 2700},    {5, 6500},    {6, 14000},   {7, 23000},
    {8, 34000},   {9, 48000},   {10, 64000},  {11, 85000},  {12, 100000}, {13, 120000}, {14, 140000},
    {15, 165000}, {16, 195000}, {17, 225000}, {18, 265000}, {19, 305000}, {20, 355000},
};

class Character : public Creature {
public:
    // TODO: should these pointers be non-const?
    const CharacterClass* class_ptr;
    const CharacterSubclass* subclass_ptr;
    const CharacterRace* race_ptr;
    const CharacterSubrace* subrace_ptr;
    Character(const std::string& name, const std::array<int, 6>& base_ability_scores) noexcept;
    Character(
        const std::string& name, const std::array<int, 6>& base_ability_scores, int level, int xp,
        const std::vector<int>& hit_dice_rolls
    ) noexcept;
    int getLevel() const noexcept;
    int getXP() const noexcept;
    void levelUp();
    void setLevel(int new_level);
    void setXP(int new_xp);
    void increaseXP(int xp_increase);
    void decreaseXP(int xp_decrease);
    const std::vector<int>& getHitDiceRolls() const noexcept;
    static int levelForXP(int xp);
    void addHitDiceRoll(int hit_dice_roll);
    virtual void determineState();
    const std::vector<const Feature*>& activeFeatures() const noexcept;
    std::vector<const Feature*> allFeatures() const;
protected:
    virtual const std::unordered_map<std::string, int> getConstants() const;
    virtual const std::unordered_map<std::string, int> getInitialAttributeValues() const;
private:
    int level, xp;
    std::vector<int> hit_dice_rolls;
    void updateLevel();
};

inline Character::Character(const std::string& name, const std::array<int, 6>& base_ability_scores) noexcept
    : Creature(name, base_ability_scores), subclass_ptr(nullptr), subrace_ptr(nullptr) {}


inline Character::Character(
    const std::string& name, const std::array<int, 6>& base_ability_scores, int level, int xp,
    const std::vector<int>& hit_dice_rolls
) noexcept
    : Creature(name, base_ability_scores), level(level), xp(xp), hit_dice_rolls(hit_dice_rolls), subclass_ptr(nullptr),
      subrace_ptr(nullptr) {}

inline int Character::getLevel() const noexcept { return level; }

inline int Character::getXP() const noexcept { return xp; }

inline void Character::updateLevel() { level = levelForXP(xp); }

inline void Character::levelUp() {
    if (level == 20) {
        std::cerr << "Warning: Cannot level up beyond level 20.\n"; // TODO: consider throwing exception instead
        return;
    }
    xp = xp_for_level.at(++level);
}

inline void Character::setLevel(int new_level) {
    if (new_level < 1 || new_level > 20) {
        std::cerr << "Warning: Level must be between 1 and 20.\n"; // TODO: consider throwing exception instead
        return;
    }
    level = new_level;
    xp = xp_for_level.at(level);
}

inline void Character::setXP(int new_xp) {
    if (new_xp < 0) {
        std::cerr << "Warning: Cannot set XP to less than 0.\n"; // TODO: consider throwing exception instead
        return;
    }
    xp = new_xp;
    updateLevel();
}

inline void Character::increaseXP(int xp_increase) {
    if (-xp_increase > xp) {
        std::cerr << "Warning: Cannot decrease XP to less than 0.\n"; // TODO: consider throwing exception instead
        return;
    }
    xp += xp_increase;
    updateLevel();
}

inline void Character::decreaseXP(int xp_decrease) { increaseXP(-xp_decrease); }

inline const std::vector<int>& Character::getHitDiceRolls() const noexcept { return hit_dice_rolls; }

inline void Character::addHitDiceRoll(int hit_dice_roll) {
    hit_dice_rolls.push_back(hit_dice_roll);
    if (hit_dice_rolls.size() > level) {
        std::cerr << "Warning: More hit dice rolls than level.\n";
    }
}

inline const std::vector<const Feature*>& Character::activeFeatures() const noexcept { return state.active_features; }

} // namespace dnd

#endif // CHARACTER_HPP_
