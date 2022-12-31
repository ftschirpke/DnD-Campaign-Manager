#ifndef ACTION_HOLDER_HPP_
#define ACTION_HOLDER_HPP_

#include <string>
#include <unordered_map>

namespace dnd {

// This class everything a creature can actively use i.e. actions, bonus actions, reactions, etc.
class ActionHolder {
public:
    std::unordered_map<std::string, std::string> actions, bonus_actions, reactions;
};

} // namespace dnd

#endif // ACTION_HOLDER_HPP_
