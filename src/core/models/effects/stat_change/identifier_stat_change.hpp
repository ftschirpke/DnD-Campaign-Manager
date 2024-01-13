#ifndef IDENTIFIER_STAT_CHANGE_HPP_
#define IDENTIFIER_STAT_CHANGE_HPP_

#include <dnd_config.hpp>

#include <string>
#include <string_view>

#include <core/errors/errors.hpp>
#include <core/models/effects/stat_change/stat_change.hpp>

namespace dnd {

/**
 * @brief A stat change that uses a value identifier to find the value to use in the operation
 */
class IdentifierStatChange : public StatChange {
public:
    IdentifierStatChange(
        const std::string& affected_attribute, StatChangeTime time, StatChangeOperation operation,
        const std::string& value_identifier
    );
    IdentifierStatChange(
        std::string_view affected_attribute, StatChangeTime time, StatChangeOperation operation,
        std::string_view value_identifier
    );
    Errors apply(Stats& stats) const override final;
private:
    std::string value_identifier;
};

} // namespace dnd

#endif // IDENTIFIER_STAT_CHANGE_HPP_
