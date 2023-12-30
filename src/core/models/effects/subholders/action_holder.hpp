#ifndef ACTION_HOLDER_HPP_
#define ACTION_HOLDER_HPP_

#include <dnd_config.hpp>

#include <map>
#include <string>

#include <core/utils/data_result.hpp>
#include <core/validation/effects/subholders/action_holder_data.hpp>

namespace dnd {

/**
 * @brief A class to hold everything a creature can actively use i.e. actions, bonus actions, reactions
 */
class ActionHolder {
public:
    using Data = ActionHolderData;

    static CreateResult<ActionHolder> create(Data&& data);

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
