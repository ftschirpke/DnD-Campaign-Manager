#include <dnd_config.hpp>

#include "literal_stat_change.hpp"

#include <string>
#include <string_view>

#include <core/errors/errors.hpp>
#include <core/models/effects/stat_change/stat_change.hpp>

namespace dnd {

LiteralStatChange::LiteralStatChange(
    const std::string& affected_attribute, StatChangeTime time, StatChangeOperation operation, bool value
)
    : StatChange(affected_attribute, time, operation), value(static_cast<int>(value)) {}

LiteralStatChange::LiteralStatChange(
    std::string_view affected_attribute, StatChangeTime time, StatChangeOperation operation, bool value
)
    : StatChange(affected_attribute, time, operation), value(static_cast<int>(value)) {}

LiteralStatChange::LiteralStatChange(
    const std::string& affected_attribute, StatChangeTime time, StatChangeOperation operation, int value
)
    : StatChange(affected_attribute, time, operation), value(value * 100) {}

LiteralStatChange::LiteralStatChange(
    std::string_view affected_attribute, StatChangeTime time, StatChangeOperation operation, int value
)
    : StatChange(affected_attribute, time, operation), value(value * 100) {}

LiteralStatChange::LiteralStatChange(
    const std::string& affected_attribute, StatChangeTime time, StatChangeOperation operation, float value
)
    : StatChange(affected_attribute, time, operation), value(static_cast<int>(value * 100)) {}

LiteralStatChange::LiteralStatChange(
    std::string_view affected_attribute, StatChangeTime time, StatChangeOperation operation, float value
)
    : StatChange(affected_attribute, time, operation), value(static_cast<int>(value * 100)) {}

Errors LiteralStatChange::apply(Stats& stats) const { return apply_with_value(stats, value); }

} // namespace dnd
