#include <dnd_config.hpp>

#include "validation_subdata.hpp"

#include <core/validation/validation_data.hpp>

namespace dnd {

Errors ValidationSubdata::validate_relations(const Content& content) const {
    DND_UNUSED(content);
    return Errors();
}

const ValidationData* ValidationSubdata::get_parent() const noexcept { return parent; }

ValidationSubdata::ValidationSubdata(const ValidationData* parent) noexcept : parent(parent) {}

} // namespace dnd
