#include <dnd_config.hpp>

#include "validation_subdata.hpp"

#include <core/validation/validation_data.hpp>

dnd::Errors dnd::ValidationSubdata::validate_relations(const Content& content) const {
    DND_UNUSED(content);
    return Errors();
}

const dnd::ValidationData* dnd::ValidationSubdata::get_parent() const noexcept { return parent; }

dnd::ValidationSubdata::ValidationSubdata(const dnd::ValidationData* parent) noexcept : parent(parent) {}
