#include <dnd_config.hpp>

#include "feature_providers_data.hpp"

#include <string>

#include <fmt/format.h>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/models/character/feature_providers.hpp>

namespace dnd {

static Errors validate_feature_providers_raw(const FeatureProviders::Data& data) {
    Errors errors;
    if (data.species_name.empty()) {
        errors.add_validation_error(ValidationError::Code::INVALID_ATTRIBUTE_VALUE, "Species name cannot be empty.");
    }
    if (data.class_name.empty()) {
        errors.add_validation_error(ValidationError::Code::INVALID_ATTRIBUTE_VALUE, "Class name cannot be empty.");
    }
    return errors;
}

static Errors validate_feature_providers_relations(const FeatureProviders::Data& data, const Content& content) {
    Errors errors;
    OptCRef<Species> species_optional = content.get_species().get(data.species_name);
    if (!species_optional.has_value()) {
        errors.add_validation_error(
            ValidationError::Code::RELATION_NOT_FOUND, fmt::format("Species '{}' does not exist.", data.species_name)
        );
    } else if (!data.subspecies_name.empty()) {
        const Species& species = species_optional.value();
        if (!species.has_subspecies()) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_RELATION,
                fmt::format("Species '{}' does not have subspecies.", data.species_name)
            );
        }
        OptCRef<Subspecies> subspecies_optional = content.get_subspecies().get(data.subspecies_name);
        if (!subspecies_optional.has_value()) {
            errors.add_validation_error(
                ValidationError::Code::RELATION_NOT_FOUND,
                fmt::format("Subspecies '{}' does not exist.", data.subspecies_name)
            );
        } else if (&subspecies_optional.value().get().get_species().get() != &species) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_RELATION,
                fmt::format(
                    "Subspecies '{}' is not a subspecies of species '{}'.", data.subspecies_name, data.species_name
                )
            );
        }
    } else if (species_optional.value().get().has_subspecies()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_RELATION, fmt::format("Species '{}' requires subspecies.", data.species_name)
        );
    }

    OptCRef<Class> class_optional = content.get_classes().get(data.class_name);
    if (!class_optional.has_value()) {
        errors.add_validation_error(
            ValidationError::Code::RELATION_NOT_FOUND, fmt::format("Class '{}' does not exist.", data.class_name)
        );
    } else if (!data.subclass_name.empty()) {
        const Class& cls = class_optional.value();
        OptCRef<Subclass> subclass_optional = content.get_subclasses().get(data.subclass_name);
        if (!subclass_optional.has_value()) {
            errors.add_validation_error(
                ValidationError::Code::RELATION_NOT_FOUND,
                fmt::format("Subclass '{}' does not exist.", data.subclass_name)
            );
        } else if (&subclass_optional.value().get().get_class().get() != &cls) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_RELATION,
                fmt::format("Subclass '{}' is not a subclass of class '{}'.", data.subclass_name, data.class_name)
            );
        }
    }
    return errors;
}

Errors validate_feature_providers_for_content(const FeatureProviders::Data& data, const Content& content) {
    Errors errors = validate_feature_providers_raw(data);
    errors += validate_feature_providers_relations(data, content);
    return errors;
}

} // namespace dnd
