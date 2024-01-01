#ifndef ACTION_HOLDER_DATA_HPP_
#define ACTION_HOLDER_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <map>
#include <string>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class ActionHolderData : public ValidationSubdata {
public:
    ActionHolderData(std::shared_ptr<const ValidationData> parent) noexcept;
    std::strong_ordering operator<=>(const ActionHolderData&) const noexcept = default;
    bool empty() const noexcept;

    std::map<std::string, std::string> actions;
    std::map<std::string, std::string> bonus_actions;
    std::map<std::string, std::string> reactions;
};

Errors validate_actions_holder(const ActionHolderData& data);

} // namespace dnd

#endif // ACTION_HOLDER_DATA_HPP_
