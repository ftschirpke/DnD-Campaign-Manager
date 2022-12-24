#ifndef CREATURE_HPP_
#define CREATURE_HPP_

#include <array>
#include <string>

#include "models/creature_state.hpp"
#include "models/feature_holder.hpp"

namespace dnd {

class Creature : public FeatureHolder {
protected:
    virtual const std::unordered_map<std::string, int> getConstants() const;
    virtual const std::unordered_map<std::string, int> getInitialAttributeValues() const;
public:
    const std::array<int, 6> base_ability_scores;
    CreatureState state;
    // TODO: probably needs a revamp, when monsters are introduced
    // TODO: actions, bonus actions, reactions? - or are they part of the state?
    Creature(const std::string& name, const std::array<int, 6>& base_ability_scores);
    virtual void determineState();
};

inline Creature::Creature(const std::string& name, const std::array<int, 6>& base_ability_scores)
    : dnd::FeatureHolder(name), base_ability_scores(base_ability_scores) {}

inline void Creature::determineState() { state = CreatureState(getConstants(), getInitialAttributeValues()); }

} // namespace dnd

#endif // CREATURE_HPP_
