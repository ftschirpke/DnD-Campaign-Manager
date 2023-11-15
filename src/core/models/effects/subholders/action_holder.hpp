#ifndef ACTION_HOLDER_HPP_
#define ACTION_HOLDER_HPP_

#include <dnd_config.hpp>

#include <map>
#include <string>

#include <core/validation/effects/subholders/action_holder_data.hpp>

namespace dnd {

/**
 * @brief A class to hold everything a creature can actively use i.e. actions, bonus actions, reactions
 */
class ActionHolder {
public:
    /**
     * @brief Constructs an ActionHolder from the given data
     * @param data the data to construct the ActionHolder from
     * @return the constructed ActionHolder
     * @throws dnd::invalid_data if the given data is invalid
     */
    static ActionHolder create(ActionHolderData&& data);

    const std::map<std::string, std::string>& get_actions() const noexcept;
    const std::map<std::string, std::string>& get_bonus_actions() const noexcept;
    const std::map<std::string, std::string>& get_reactions() const noexcept;

    bool empty() const;
    void merge(ActionHolder&& other);
private:
    ActionHolder(
        std::map<std::string, std::string>&& actions, std::map<std::string, std::string>&& bonus_actions,
        std::map<std::string, std::string>&& reactions
    ) noexcept;

    std::map<std::string, std::string> actions;
    std::map<std::string, std::string> bonus_actions;
    std::map<std::string, std::string> reactions;
};

} // namespace dnd

#endif // ACTION_HOLDER_HPP_
