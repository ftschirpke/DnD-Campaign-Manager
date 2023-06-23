#ifndef ACTION_HOLDER_DATA_HPP_
#define ACTION_HOLDER_DATA_HPP_

#include <dnd_config.hpp>

#include <map>
#include <string>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class ActionHolderData : public ValidationSubdata {
public:
    ActionHolderData(const ValidationData* parent) noexcept;
    int operator<=>(const ActionHolderData&) const noexcept = default;
    /**
     * @brief Validates the data
     * @return the errors that occured during validation
     */
    virtual Errors validate() const override;
    /**
     * @brief Checks if the action holder is empty
     * @return "true" if the action holder is empty, "false" otherwise
     */
    bool empty() const noexcept;

    std::map<std::string, std::string> actions;
    std::map<std::string, std::string> bonus_actions;
    std::map<std::string, std::string> reactions;
};

} // namespace dnd

#endif // ACTION_HOLDER_DATA_HPP_
