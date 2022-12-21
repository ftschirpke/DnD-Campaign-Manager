#ifndef CREATURE_HPP_
#define CREATURE_HPP_

#include <string>

#include "creature_state.hpp"

namespace dnd {

class Creature {
public:
    const std::string name;
    const std::vector<int> base_ability_scores;
    CreatureState state;
    // TODO: probably needs a revamp, when monsters are introduced
    // TODO: actions, bonus actions, reactions? - or are they part of the state?
    Creature(const std::string& name, const std::vector<int>& base_ability_scores)
        : name(name), base_ability_scores(base_ability_scores) {}
};

} // namespace dnd

#endif // CREATURE_HPP_
