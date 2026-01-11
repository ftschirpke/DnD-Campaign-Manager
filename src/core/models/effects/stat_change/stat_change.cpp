#include <dnd_config.hpp>

#include "stat_change.hpp"

#include <cassert>
#include <string>
#include <string_view>

#include <core/errors/errors.hpp>
#include <core/errors/runtime_error.hpp>
#include <core/models/character/stats.hpp>

namespace dnd {

StatChangeTime StatChange::get_time() const { return time; }

const std::string& StatChange::get_affected_attribute() const { return affected_attribute; }

StatChange::StatChange(const std::string& affected_attribute, StatChangeTime time, StatChangeOperation operation)
    : affected_attribute(affected_attribute), operation(operation), time(time) {}

StatChange::StatChange(std::string_view affected_attribute, StatChangeTime time, StatChangeOperation operation)
    : affected_attribute(affected_attribute), operation(operation), time(time) {}

Errors StatChange::apply_with_value(Stats& stats, int value) const {
    Errors errors;
    int& affected_stat = stats.get_raw_mut_or_insert(affected_attribute);
    switch (operation) {
        case StatChangeOperation::ADD:
            affected_stat += value;
            return errors;
        case StatChangeOperation::SUB:
            affected_stat -= value;
            return errors;
        case StatChangeOperation::MULT:
            affected_stat *= value;
            affected_stat /= 100; // HACK: balance the '* 100' applied to values
            return errors;
        case StatChangeOperation::DIV:
            if (value == 0) {
                errors.add_runtime_error(RuntimeError::Code::INVALID_ARGUMENT, "Cannot divide by zero.");
            } else {
                affected_stat *= 100; // HACK: balance the '* 100' applied to values
                affected_stat /= value;
            }
            return errors;
        case StatChangeOperation::SET:
            affected_stat = value;
            return errors;
        case StatChangeOperation::MAX:
            affected_stat = std::max(affected_stat, value);
            return errors;
        case StatChangeOperation::MIN:
            affected_stat = std::min(affected_stat, value);
            return errors;
    }
    std::unreachable();
}

} // namespace dnd
