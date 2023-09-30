#include <dnd_config.hpp>

#include "content_filter.hpp"

#include <string>

bool dnd::ContentFilter::matches_string(
    const std::string& str, StringFilterType type, const std::string& filter
) noexcept {
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

bool dnd::ContentFilter::matches_int(int number, NumberFilterType type, int filter) noexcept {
    switch (type) {
        case NumberFilterType::EQUAL:
            return number == filter;
        case NumberFilterType::NOT_EQUAL:
            return number != filter;
        case NumberFilterType::LESS_THAN:
            return number < filter;
        case NumberFilterType::LESS_THAN_OR_EQUAL:
            return number <= filter;
        case NumberFilterType::GREATER_THAN:
            return number > filter;
        case NumberFilterType::GREATER_THAN_OR_EQUAL:
            return number >= filter;
        case NumberFilterType::NONE:
            return true;
        default:
            return false;
    }
}

bool dnd::ContentFilter::matches_float(float number, NumberFilterType type, float filter) noexcept {
    switch (type) {
        case NumberFilterType::EQUAL:
            return number == filter;
        case NumberFilterType::NOT_EQUAL:
            return number != filter;
        case NumberFilterType::LESS_THAN:
            return number < filter;
        case NumberFilterType::LESS_THAN_OR_EQUAL:
            return number <= filter;
        case NumberFilterType::GREATER_THAN:
            return number > filter;
        case NumberFilterType::GREATER_THAN_OR_EQUAL:
            return number >= filter;
        case NumberFilterType::NONE:
            return true;
        default:
            return false;
    }
}

bool dnd::ContentFilter::matches_bool(bool boolean, BoolFilterType type) noexcept {
    switch (type) {
        case BoolFilterType::IS_TRUE:
            return boolean;
        case BoolFilterType::IS_FALSE:
            return !boolean;
        case BoolFilterType::NONE:
            return true;
        default:
            return false;
    }
}
