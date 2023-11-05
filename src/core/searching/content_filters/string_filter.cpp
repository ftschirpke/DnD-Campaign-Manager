#include <dnd_config.hpp>

#include "string_filter.hpp"

#include <string>

dnd::StringFilter::StringFilter() noexcept : type(StringFilterType::NONE), value() {}

bool dnd::StringFilter::is_set() const noexcept { return type != StringFilterType::NONE; }

dnd::StringFilterType dnd::StringFilter::get_type() const noexcept { return type; }

std::string& dnd::StringFilter::get_value() noexcept { return value; }

const std::string& dnd::StringFilter::get_value() const noexcept { return value; }

void dnd::StringFilter::set_type(StringFilterType type) noexcept { this->type = type; }

void dnd::StringFilter::set_value(const std::string& value) noexcept { this->value = value; }

void dnd::StringFilter::set_value(std::string&& value) noexcept { this->value = value; }

void dnd::StringFilter::set(StringFilterType type, const std::string& value) noexcept {
    set_type(type);
    set_value(value);
}

void dnd::StringFilter::set(StringFilterType type, std::string&& value) noexcept {
    set_type(type);
    set_value(std::move(value));
}

void dnd::StringFilter::clear() noexcept {
    set_type(StringFilterType::NONE);
    set_value("");
}

bool dnd::StringFilter::matches(const std::string& str) const noexcept {
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
