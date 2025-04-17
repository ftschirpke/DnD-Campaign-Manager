#include <dnd_config.hpp>

#include "feature_providers_validation.hpp"

#include <string>

#include <fmt/format.h>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/models/character/feature_providers.hpp>

namespace dnd {

static Errors validate_feature_providers_raw(const FeatureProviders::Data& data) {
    Errors errors;
    if (data.species_key.empty()) {
        errors.add_validation_error(ValidationError::Code::INVALID_ATTRIBUTE_VALUE, "Species name cannot be empty.");
    }
    if (data.class_key.empty()) {
        errors.add_validation_error(ValidationError::Code::INVALID_ATTRIBUTE_VALUE, "Class name cannot be empty.");
    }
    return errors;
}

static Errors validate_feature_providers_relations(const FeatureProviders::Data& data, const Content& content) {
    Errors errors;

    OptCRef<Species> species_optional = content.get_species().get(data.species_key);
    if (!species_optional.has_value()) {
        errors.add_validation_error(
            ValidationError::Code::RELATION_NOT_FOUND, fmt::format("Species '{}' does not exist.", data.species_key)
        );
    } else if (!data.subspecies_key.empty()) {
        const Species& species = species_optional.value();
        OptCRef<Subspecies> subspecies_optional = content.get_subspecies().get(data.subspecies_key);
        if (!subspecies_optional.has_value()) {
            errors.add_validation_error(
                ValidationError::Code::RELATION_NOT_FOUND,
                fmt::format("Subspecies '{}' does not exist.", data.subspecies_key)
            );
        } else if (&subspecies_optional.value().get().get_species().get() != &species) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_RELATION,
                fmt::format(
                    "Subspecies '{}' is not a subspecies of species '{}'.", data.subspecies_key, data.species_key
                )
            );
        }
    }
    // TODO: potentially check for missing subspecies

    OptCRef<Class> class_optional = content.get_classes().get(data.class_key);
    if (!class_optional.has_value()) {
        errors.add_validation_error(
            ValidationError::Code::RELATION_NOT_FOUND, fmt::format("Class '{}' does not exist.", data.class_key)
        );
    } else if (!data.subclass_key.empty()) {
        const Class& cls = class_optional.value();
        OptCRef<Subclass> subclass_optional = content.get_subclasses().get(data.subclass_key);
        if (!subclass_optional.has_value()) {
            errors.add_validation_error(
                ValidationError::Code::RELATION_NOT_FOUND,
                fmt::format("Subclass '{}' does not exist.", data.subclass_key)
            );
        } else if (&subclass_optional.value().get().get_class().get() != &cls) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_RELATION,
                fmt::format("Subclass '{}' is not a subclass of class '{}'.", data.subclass_key, data.class_key)
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
