#include <dnd_config.hpp>

#include "string_filtering.hpp"

#include <string>

bool dnd::matches_string(const std::string& str, StringFilterType type, const std::string& filter) noexcept {
    switch (type) {
        case StringFilterType::EQUAL:
            return str == filter;
        case StringFilterType::NOT_EQUAL:
            return str != filter;
        case StringFilterType::CONTAINS:
            return str.find(filter) != std::string::npos;
        case StringFilterType::NOT_CONTAINS:
            return str.find(filter) == std::string::npos;
        case StringFilterType::STARTS_WITH:
            return str.find(filter) == 0;
        case StringFilterType::NOT_STARTS_WITH:
            return str.find(filter) != 0;
        case StringFilterType::ENDS_WITH:
            return str.rfind(filter) == str.size() - filter.size();
        case StringFilterType::NOT_ENDS_WITH:
            return str.rfind(filter) != str.size() - filter.size();
        case StringFilterType::NONE:
            return true;
        default:
            return false;
    }
}
