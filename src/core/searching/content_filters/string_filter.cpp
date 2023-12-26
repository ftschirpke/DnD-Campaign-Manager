#include <dnd_config.hpp>

#include "string_filter.hpp"

#include <string>

namespace dnd {

StringFilter::StringFilter() noexcept : type(StringFilterType::NONE), value() {}

bool StringFilter::is_set() const noexcept { return type != StringFilterType::NONE; }

StringFilterType StringFilter::get_type() const noexcept { return type; }

std::string& StringFilter::get_value_mutable() noexcept { return value; }

const std::string& StringFilter::get_value() const noexcept { return value; }

void StringFilter::set_type(StringFilterType new_type) noexcept { type = new_type; }

void StringFilter::set_value(const std::string& new_value) noexcept { value = new_value; }

void StringFilter::set_value(std::string&& new_value) noexcept { value = new_value; }

void StringFilter::set(StringFilterType new_type, const std::string& new_value) noexcept {
    set_type(new_type);
    set_value(new_value);
}

void StringFilter::set(StringFilterType new_type, std::string&& new_value) noexcept {
    set_type(new_type);
    set_value(std::move(new_value));
}

void StringFilter::clear() noexcept {
    set_type(StringFilterType::NONE);
    set_value("");
}

bool StringFilter::matches(const std::string& str) const noexcept {
    switch (type) {
        case StringFilterType::EQUAL:
            return str == value;
        case StringFilterType::NOT_EQUAL:
            return str != value;
        case StringFilterType::CONTAINS:
            return str.find(value) != std::string::npos;
        case StringFilterType::NOT_CONTAINS:
            return str.find(value) == std::string::npos;
        case StringFilterType::STARTS_WITH:
            return str.find(value) == 0;
        case StringFilterType::NOT_STARTS_WITH:
            return str.find(value) != 0;
        case StringFilterType::ENDS_WITH:
            return str.rfind(value) == str.size() - value.size();
        case StringFilterType::NOT_ENDS_WITH:
            return str.rfind(value) != str.size() - value.size();
        case StringFilterType::NONE:
            return true;
        default:
            return false;
    }
}

} // namespace dnd
