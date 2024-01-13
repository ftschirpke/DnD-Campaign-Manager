#ifndef LITERAL_STAT_CHANGE_HPP_
#define LITERAL_STAT_CHANGE_HPP_

#include <dnd_config.hpp>

#include <string>
#include <string_view>

#include <core/errors/errors.hpp>
#include <core/models/effects/stat_change/stat_change.hpp>

namespace dnd {

/**
 * @brief A class that represents a stat change that modifies a value by a literal (boolean, integer, or float)
 */
class LiteralStatChange : public StatChange {
public:
    LiteralStatChange(
        const std::string& affected_attribute, StatChangeTime time, StatChangeOperation operation, bool value
    );
    LiteralStatChange(
        std::string_view affected_attribute, StatChangeTime time, StatChangeOperation operation, bool value
    );
    LiteralStatChange(
        const std::string& affected_attribute, StatChangeTime time, StatChangeOperation operation, int value
    );
    LiteralStatChange(
        std::string_view affected_attribute, StatChangeTime time, StatChangeOperation operation, int value
    );
    LiteralStatChange(
        const std::string& affected_attribute, StatChangeTime time, StatChangeOperation operation, float value
    );
    LiteralStatChange(
        std::string_view affected_attribute, StatChangeTime time, StatChangeOperation operation, float value
    );

    Errors apply(Stats& stats) const override final;
private:
    int value;
};

} // namespace dnd

#endif // LITERAL_STAT_CHANGE_HPP_
