#ifndef CHARACTER_HPP_
#define CHARACTER_HPP_

#include <iostream>
#include <memory>
#include <unordered_map>

#include "character_class.hpp"
#include "character_race.hpp"
#include "creature.hpp"

namespace dnd {

const std::unordered_map<int, int> xp_for_level = {
    {1, 0},       {2, 300},     {3, 900},     {4, 2700},    {5, 6500},    {6, 14000},   {7, 23000},
    {8, 34000},   {9, 48000},   {10, 64000},  {11, 85000},  {12, 100000}, {13, 120000}, {14, 140000},
    {15, 165000}, {16, 195000}, {17, 225000}, {18, 265000}, {19, 305000}, {20, 355000},
};

class Character : public Creature {
private:
    int xp, level;
public:
    // TODO: should these pointers be non-const?
    std::shared_ptr<const CharacterClass> class_ptr;
    std::shared_ptr<const CharacterSubclass> subclass_ptr;
    std::shared_ptr<const CharacterRace> race_ptr;
    std::shared_ptr<const CharacterSubrace> subrace_ptr;
    Character(const std::string& name) : Creature(name) {}
    inline int getLevel() const { return level; }
    inline int getXp() const { return level; }
    inline void levelUp() {
        if (level == 20) {
            std::cerr << "Warning: Cannot level up beyond level 20.\n"; // TODO: consider throwing exception instead
            return;
        }
        xp = xp_for_level.at(++level);
    }
    inline void setXP(int new_xp) {
        if (new_xp < 0) {
            std::cerr << "Warning: Cannot set XP to less than 0.\n"; // TODO: consider throwing exception instead
            return;
        }
        xp = new_xp;
    }
    inline void increaseXP(int xp_increase) {
        if (-xp_increase > xp) {
            std::cerr << "Warning: Cannot decrease XP to less than 0.\n"; // TODO: consider throwing exception instead
            return;
        }
        xp += xp_increase;
    }
    inline void decreaseXP(int xp_decrease) { increaseXP(-xp_decrease); }
};

} // namespace dnd

#endif // CHARACTER_HPP_
