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

    Opt<CRef<Species>> species_optional = content.get_species_library().get(data.species_key);
    if (!species_optional.has_value()) {
        errors.add_validation_error(
            ValidationError::Code::RELATION_NOT_FOUND, fmt::format("Species '{}' does not exist.", data.species_key)
        );
    } else if (!data.subspecies_key.empty()) {
        const Species& species = species_optional.value();
        Opt<CRef<Subspecies>> subspecies_optional = content.get_subspecies_library().get(data.subspecies_key);
        if (!subspecies_optional.has_value()) {
            errors.add_validation_error(
                ValidationError::Code::RELATION_NOT_FOUND,
                fmt::format("Subspecies '{}' does not exist.", data.subspecies_key)
            );
        } else if (subspecies_optional.value().get().get_species().get().get_name() != species.get_name()) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_RELATION,
                fmt::format(
                    "Subspecies '{}' is not a subspecies of species '{}'.", data.subspecies_key, species.get_name()
                )
            );
        }
    }
    // TODO: potentially check for missing subspecies

    Opt<Id> class_id = content.find_class(data.class_key);
    if (!class_id.has_value()) {
        errors.add_validation_error(
            ValidationError::Code::RELATION_NOT_FOUND, fmt::format("Class '{}' does not exist.", data.class_key)
        );
    } else if (!data.subclass_key.empty()) {
        const Class& cls = content.get_class(class_id.value());
        Opt<Id> subclass_id = content.find_subclass(data.subclass_key);
        if (!subclass_id.has_value()) {
            errors.add_validation_error(
                ValidationError::Code::RELATION_NOT_FOUND,
                fmt::format("Subclass '{}' does not exist.", data.subclass_key)
            );
        } else {
            const Subclass& subclass = content.get_subclass(subclass_id.value());
            const Class& subclass_class = content.get_class(subclass.get_class_id());
            if (subclass_class.get_name() != cls.get_name()) {
                errors.add_validation_error(
                    ValidationError::Code::INVALID_RELATION,
                    fmt::format("Subclass '{}' is not a subclass of class '{}'.", data.subclass_key, cls.get_name())
                );
            }
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
