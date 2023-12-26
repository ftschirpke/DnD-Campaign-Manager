#include <dnd_config.hpp>

#include "stat_change_factory.hpp"

#include <algorithm>
#include <cassert>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>

#include <core/models/effects/stat_change/identifier_stat_change.hpp>
#include <core/models/effects/stat_change/literal_stat_change.hpp>
#include <core/utils/string_manipulation.hpp>
#include <core/validation/effects/stat_change/stat_change_data.hpp>

namespace dnd {

std::unique_ptr<StatChange> create_stat_change(StatChangeData&& stat_change_data) {
    if (!stat_change_data.validate().ok()) {
        throw invalid_data("Cannot create stat_change from invalid data.");
    }
    std::string::const_iterator it = std::find(
        stat_change_data.stat_change_str.cbegin(), stat_change_data.stat_change_str.cend(), ' '
    );
    const std::string_view affected_attribute = str_view(stat_change_data.stat_change_str.cbegin(), it);
    std::string::const_iterator last_it = ++it;
    it = std::find(it, stat_change_data.stat_change_str.cend(), ' ');

    const std::string_view stat_change_time_str = str_view(last_it, it);
    StatChangeTime stat_change_time = static_cast<StatChangeTime>(-1);
    if (stat_change_time_str == "earliest") {
        stat_change_time = StatChangeTime::EARLIEST;
    } else if (stat_change_time_str == "early") {
        stat_change_time = StatChangeTime::EARLY;
    } else if (stat_change_time_str == "normal") {
        stat_change_time = StatChangeTime::NORMAL;
    } else if (stat_change_time_str == "late") {
        stat_change_time = StatChangeTime::LATE;
    } else if (stat_change_time_str == "latest") {
        stat_change_time = StatChangeTime::LATEST;
    }
    assert(stat_change_time != static_cast<StatChangeTime>(-1));

    last_it = ++it;
    it = std::find(it, stat_change_data.stat_change_str.cend(), ' ');
    const std::string_view operator_name = str_view(last_it, it);
    const std::string_view value = str_view(++it, stat_change_data.stat_change_str.cend());

    if (value[0] >= 'A' && value[0] <= 'Z') {
        return std::make_unique<IdentifierStatChange>(affected_attribute, stat_change_time, operator_name, value);
    } else if (value == "true" || value == "false") {
        return std::make_unique<LiteralStatChange>(
            affected_attribute, stat_change_time, operator_name, value == "true"
        );
    } else if (value.find('.') != std::string::npos) {
        return std::make_unique<LiteralStatChange>(
            affected_attribute, stat_change_time, operator_name, std::stof(value.data())
        );
    } else {
        return std::make_unique<LiteralStatChange>(
            affected_attribute, stat_change_time, operator_name, std::stoi(value.data())
        );
    }
}

} // namespace dnd
