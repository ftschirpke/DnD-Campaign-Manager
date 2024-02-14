#include <dnd_config.hpp>

#include "identifier_stat_change.hpp"

#include <optional>
#include <string>
#include <string_view>

#include <fmt/format.h>

#include <core/errors/errors.hpp>
#include <core/errors/runtime_error.hpp>
#include <core/models/character/stats.hpp>
#include <core/models/effects/stat_change/stat_change.hpp>

namespace dnd {

IdentifierStatChange::IdentifierStatChange(
    const std::string& affected_attribute, StatChangeTime time, StatChangeOperation operation,
    const std::string& value_identifier
)
    : StatChange(affected_attribute, time, operation), value_identifier(value_identifier) {}

IdentifierStatChange::IdentifierStatChange(
    std::string_view affected_attribute, StatChangeTime time, StatChangeOperation operation,
    std::string_view value_identifier
)
    : StatChange(affected_attribute, time, operation), value_identifier(value_identifier) {}

Errors IdentifierStatChange::apply(Stats& stats) const {
    std::optional<int> value_optional = stats.get_raw(value_identifier);
    if (!value_optional.has_value()) {
        return Errors(RuntimeError(
            RuntimeError::Code::INVALID_ARGUMENT,
            fmt::format("Identifier for stat change value '{}' not found in stats", value_identifier)
        ));
    }
    return apply_with_value(stats, value_optional.value());
}

} // namespace dnd
