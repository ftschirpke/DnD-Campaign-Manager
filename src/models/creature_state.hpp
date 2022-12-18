#ifndef CREATURE_STATE_HPP_
#define CREATURE_STATE_HPP_

#include <string>
#include <unordered_map>

namespace dnd {

class CreatureState {
public:
    const std::unordered_map<std::string, int> constants;
    std::unordered_map<std::string, int> attributes;
};

} // namespace dnd

#endif // CREATURE_STATE_HPP_
