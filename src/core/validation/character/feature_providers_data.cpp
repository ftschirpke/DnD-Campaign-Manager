#include <dnd_config.hpp>

#include "feature_providers_data.hpp"

#include <string>

#include <fmt/format.h>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

FeatureProvidersData::FeatureProvidersData(const ValidationData* parent) noexcept : ValidationSubdata(parent) {}

Errors FeatureProvidersData::validate() const {
    Errors errors;
    if (species_name.empty()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, parent, "Species name cannot be empty."
        );
    }
    if (class_name.empty()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, parent, "Class name cannot be empty."
        );
    }
    return errors;
}

Errors FeatureProvidersData::validate_relations(const Content& content) const {
    Errors errors;
    OptCRef<Species> species_optional = content.get_species().get(species_name);
    if (!species_optional.has_value()) {
        errors.add_validation_error(
            ValidationError::Code::RELATION_NOT_FOUND, parent, fmt::format("Species '{}' does not exist.", species_name)
        );
    } else if (!subspecies_name.empty()) {
        const Species& species = species_optional.value();
        if (!species.has_subspecies()) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_RELATION, parent,
                fmt::format("Species '{}' does not have subspecies.", species_name)
            );
        }
        OptCRef<Subspecies> subspecies_optional = content.get_subspecies().get(subspecies_name);
        if (!subspecies_optional.has_value()) {
            errors.add_validation_error(
                ValidationError::Code::RELATION_NOT_FOUND, parent,
                fmt::format("Subspecies '{}' does not exist.", subspecies_name)
            );
        } else if (subspecies_optional.value().get().get_species() != &species) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_RELATION, parent,
                fmt::format("Subspecies '{}' is not a subspecies of species '{}'.", subspecies_name, species_name)
            );
        }
    } else if (species_optional.value().get().has_subspecies()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_RELATION, parent,
            fmt::format("Species '{}' requires subspecies.", species_name)
        );
    }

    OptCRef<Class> class_optional = content.get_classes().get(class_name);
    if (!class_optional.has_value()) {
        errors.add_validation_error(
            ValidationError::Code::RELATION_NOT_FOUND, parent, fmt::format("Class '{}' does not exist.", class_name)
        );
    } else if (!subclass_name.empty()) {
        const Class& cls = class_optional.value();
        OptCRef<Subclass> subclass_optional = content.get_subclasses().get(subclass_name);
        if (!subclass_optional.has_value()) {
            errors.add_validation_error(
                ValidationError::Code::RELATION_NOT_FOUND, parent,
                fmt::format("Subclass '{}' does not exist.", subclass_name)
            );
        } else if (subclass_optional.value().get().get_class() != &cls) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_RELATION, parent,
                fmt::format("Subclass '{}' is not a subclass of class '{}'.", subclass_name, class_name)
            );
        }
    }
    return errors;
}

} // namespace dnd
