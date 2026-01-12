#include <dnd_config.hpp>

#include "string_filter.hpp"

#include <cassert>
#include <string>
#include <utility>

namespace dnd {

StringFilter::StringFilter() : type(StringFilterType::NONE), value() {}

bool StringFilter::is_set() const { return type != StringFilterType::NONE; }

StringFilterType StringFilter::get_type() const { return type; }

std::string& StringFilter::get_value_mutable() { return value; }

const std::string& StringFilter::get_value() const { return value; }

void StringFilter::set_type(StringFilterType new_type) { type = new_type; }

void StringFilter::set_value(const std::string& new_value) { value = new_value; }

void StringFilter::set_value(std::string&& new_value) { value = new_value; }

void StringFilter::set(StringFilterType new_type, const std::string& new_value) {
    set_type(new_type);
    set_value(new_value);
}

void StringFilter::set(StringFilterType new_type, std::string&& new_value) {
    set_type(new_type);
    set_value(std::move(new_value));
}

void StringFilter::clear() {
    set_type(StringFilterType::NONE);
    set_value("");
}

bool StringFilter::matches(const std::string& str) const {
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
    }
    std::unreachable();
}

} // namespace dnd
