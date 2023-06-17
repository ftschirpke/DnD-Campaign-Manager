#include <dnd_config.hpp>

#include "validation_subdata.hpp"

#include <core/validation/validation_data.hpp>

dnd::ValidationSubdata::ValidationSubdata(const ValidationData* parent) noexcept : parent(parent) {}

dnd::Errors dnd::ValidationSubdata::validate_relations(const ContentHolder& content) const {
    DND_UNUSED(content);
    return Errors();
}
