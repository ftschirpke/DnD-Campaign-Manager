#ifndef ACTION_HOLDER_HPP_
#define ACTION_HOLDER_HPP_

#include <dnd_config.hpp>

#include <map>
#include <string>

#include <core/utils/data_result.hpp>

namespace dnd {

/**
 * @brief A class to hold everything a creature can actively use i.e. actions, bonus actions, reactions
 */
class ActionHolder {
public:
    class Data;

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

class ActionHolder::Data {
public:
    std::strong_ordering operator<=>(const Data&) const noexcept = default;
    bool empty() const noexcept;

    std::map<std::string, std::string> actions;
    std::map<std::string, std::string> bonus_actions;
    std::map<std::string, std::string> reactions;
};

inline bool ActionHolder::Data::empty() const noexcept {
    return actions.empty() && bonus_actions.empty() && reactions.empty();
}

} // namespace dnd

#endif // ACTION_HOLDER_HPP_
