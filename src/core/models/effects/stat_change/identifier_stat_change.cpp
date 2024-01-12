#include <dnd_config.hpp>

#include "identifier_stat_change.hpp"

#include <functional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_map>

#include <core/models/effects/stat_change/stat_change.hpp>

namespace dnd {

IdentifierStatChange::IdentifierStatChange(
    const std::string& affected_attribute, StatChangeTime time, const std::string& operation_name,
    const std::string& value_identifier
)
    : StatChange(affected_attribute, time, operation_name), value_identifier(value_identifier) {}

IdentifierStatChange::IdentifierStatChange(
    std::string_view affected_attribute, StatChangeTime time, std::string_view operation_name,
    std::string_view value_identifier
)
    : StatChange(affected_attribute, time, operation_name), value_identifier(value_identifier) {}

void IdentifierStatChange::apply_to(
    std::unordered_map<std::string, int>& attributes, const std::unordered_map<std::string, int>& constants
) const {
    if (mathematical_operation == nullptr) {
        return;
    }
    int value;
    if (attributes.contains(value_identifier)) {
        value = attributes.at(value_identifier);
    } else if (constants.contains(value_identifier)) {
        value = constants.at(value_identifier);
    } else {
        throw std::out_of_range("Identifier \"" + value_identifier + "\" not found.");
    }
    attributes[affected_attribute] = mathematical_operation(attributes[affected_attribute], value);
}

} // namespace dnd
