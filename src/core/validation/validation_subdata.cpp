#include <dnd_config.hpp>

#include "validation_subdata.hpp"

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

Errors ValidationSubdata::validate_relations(const Content& content) const {
    DND_UNUSED(content);
    return Errors();
}

std::shared_ptr<const ValidationData> ValidationSubdata::get_parent() const noexcept { return std::shared_ptr(parent); }

ValidationSubdata::ValidationSubdata(std::shared_ptr<const ValidationData> parent) noexcept : parent(parent) {}

} // namespace dnd
