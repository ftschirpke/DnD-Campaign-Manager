#ifndef CREATURE_HPP_
#define CREATURE_HPP_

#include <string>

#include "creature_state.hpp"

namespace dnd {

class Creature {
public:
    const std::string name;
    CreatureState state;
    // TODO: probably needs a revamp, when monsters are introduced
    // TODO: actions, bonus actions, reactions? - or are they part of the state?
    Creature(const std::string& name) : name(name)
};

} // namespace dnd

#endif // CREATURE_HPP_
