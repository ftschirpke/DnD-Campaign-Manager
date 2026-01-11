#include <dnd_config.hpp>

#include "bool_filter.hpp"

#include <cassert>

namespace dnd {

BoolFilter::BoolFilter() : type(BoolFilterType::NONE) {}

bool BoolFilter::is_set() const { return type != BoolFilterType::NONE; }

BoolFilterType BoolFilter::get_type() const { return type; }

void BoolFilter::set_type(BoolFilterType new_type) { type = new_type; }

void BoolFilter::set(BoolFilterType new_type) { set_type(new_type); }

void BoolFilter::clear() { type = BoolFilterType::NONE; }

bool BoolFilter::matches(bool boolean) const {
    switch (type) {
        case BoolFilterType::IS_TRUE:
            return boolean;
        case BoolFilterType::IS_FALSE:
            return !boolean;
        case BoolFilterType::NONE:
            return true;
    }
    std::unreachable();
}

} // namespace dnd
