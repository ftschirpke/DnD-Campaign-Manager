#ifndef ACTION_HOLDER_HPP_
#define ACTION_HOLDER_HPP_

#include <dnd_config.hpp>

#include <map>
#include <string>

namespace dnd {

/**
 * @brief A class to hold everything a creature can actively use i.e. actions, bonus actions, reactions
 */
class ActionHolder {
public:
    /**
     * @brief Returns whether the ActionHolder is empty
     * @return "true" if the ActionHolder is empty, "false" otherwise
     */
    bool empty() const;

    // actions represented as a pair of a name (key) and a short description (value)
    std::map<std::string, std::string> actions;
    // bonus actions represented as a pair of a name (key) and a short description (value)
    std::map<std::string, std::string> bonus_actions;
    // reactions represented as a pair of a name (key) and a short description (value)
    std::map<std::string, std::string> reactions;
};

inline bool ActionHolder::empty() const { return actions.empty() && bonus_actions.empty() && reactions.empty(); }

} // namespace dnd

#endif // ACTION_HOLDER_HPP_
