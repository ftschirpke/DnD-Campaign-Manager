#include <dnd_config.hpp>

#include "literal_stat_change.hpp"

#include <functional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

#include <core/models/effects/stat_change/stat_change.hpp>

namespace dnd {

LiteralStatChange::LiteralStatChange(
    const std::string& affected_attribute, StatChangeTime time, const std::string& operation_name, bool value
)
    : StatChange(affected_attribute, time, operation_name), value(static_cast<int>(value)) {}

LiteralStatChange::LiteralStatChange(
    std::string_view affected_attribute, StatChangeTime time, std::string_view operation_name, bool value
)
    : StatChange(affected_attribute, time, operation_name), value(static_cast<int>(value)) {}

LiteralStatChange::LiteralStatChange(
    const std::string& affected_attribute, StatChangeTime time, const std::string& operation_name, int value
)
    : StatChange(affected_attribute, time, operation_name), value(value * 100) {}

LiteralStatChange::LiteralStatChange(
    std::string_view affected_attribute, StatChangeTime time, std::string_view operation_name, int value
)
    : StatChange(affected_attribute, time, operation_name), value(value * 100) {}

LiteralStatChange::LiteralStatChange(
    const std::string& affected_attribute, StatChangeTime time, const std::string& operation_name, float value
)
    : StatChange(affected_attribute, time, operation_name), value(static_cast<int>(value * 100)) {}

LiteralStatChange::LiteralStatChange(
    std::string_view affected_attribute, StatChangeTime time, std::string_view operation_name, float value
)
    : StatChange(affected_attribute, time, operation_name), value(static_cast<int>(value * 100)) {}

void LiteralStatChange::apply_to(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    DND_UNUSED(constants);
    if (mathematical_operation == nullptr) {
        return;
    }
    attributes[affected_attribute] = mathematical_operation(attributes[affected_attribute], value);
}

} // namespace dnd
