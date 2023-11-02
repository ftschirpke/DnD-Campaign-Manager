#include <dnd_config.hpp>

#include "bool_filter.hpp"

dnd::BoolFilter::BoolFilter() noexcept : type(BoolFilterType::NONE) {}

dnd::BoolFilterType dnd::BoolFilter::get_type() const noexcept { return type; }

void dnd::BoolFilter::set_type(BoolFilterType type) noexcept { this->type = type; }

void dnd::BoolFilter::set(BoolFilterType type) noexcept { set_type(type); }

void dnd::BoolFilter::clear() noexcept { type = BoolFilterType::NONE; }

bool dnd::BoolFilter::matches(bool boolean) const noexcept {
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
