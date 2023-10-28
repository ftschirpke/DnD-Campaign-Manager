#include <dnd_config.hpp>

#include "bool_filtering.hpp"

bool dnd::matches_bool(bool boolean, BoolFilterType type) noexcept {
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
