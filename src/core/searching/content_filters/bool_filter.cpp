#include <dnd_config.hpp>

#include "bool_filter.hpp"

namespace dnd {

BoolFilter::BoolFilter() noexcept : type(BoolFilterType::NONE) {}

bool BoolFilter::is_set() const noexcept { return type != BoolFilterType::NONE; }

BoolFilterType BoolFilter::get_type() const noexcept { return type; }

void BoolFilter::set_type(BoolFilterType new_type) noexcept { type = new_type; }

void BoolFilter::set(BoolFilterType new_type) noexcept { set_type(new_type); }

void BoolFilter::clear() noexcept { type = BoolFilterType::NONE; }

bool BoolFilter::matches(bool boolean) const noexcept {
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

} // namespace dnd
