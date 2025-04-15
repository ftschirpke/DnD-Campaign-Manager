#include <dnd_config.hpp>

#include "subspecies_validation.hpp"

#include <string>
#include <unordered_set>
#include <vector>

#include <fmt/format.h>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/subspecies/subspecies.hpp>
#include <core/validation/effects_provider/feature_validation.hpp>

namespace dnd {

static Errors validate_subspecies_raw_nonrecursively(const Subspecies::Data& data) {
    Errors errors;
    if (data.features_data.empty()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, "Character subspecies has no features."
        );
    }
    if (data.species_key.empty()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, "Character subspecies has no species name."
        );
    }
    return errors;
}

Errors validate_subspecies_relations_nonrecursively(const Subspecies::Data& data, const Content& content) {
    Errors errors;
    if (content.get_subspecies().contains(data.get_key())) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
            fmt::format("Subspecies has duplicate key \"{}\".", data.get_key())
        );
    }
    for (const Feature::Data& feature_data : data.features_data) {
        if (content.get_features().contains(feature_data.get_key())) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
                fmt::format("Feature has duplicate key \"{}\".", feature_data.get_key())
            );
        }
    }

    OptCRef<Species> species_optional = content.get_species().get(data.species_key);
    if (!species_optional.has_value()) {
        errors.add_validation_error(
            ValidationError::Code::RELATION_NOT_FOUND,
            fmt::format("Character species '{}' does not exist.", data.species_key)
        );
    } else if (!species_optional.value().get().has_subspecies()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_RELATION,
            fmt::format("Character species '{}' does not have subspecies.", data.species_key)
        );
    }
    return errors;
}

Errors validate_subspecies_nonrecursively_for_content(const Subspecies::Data& data, const Content& content) {
    Errors errors = validate_subspecies_raw_nonrecursively(data);
    errors += validate_subspecies_relations_nonrecursively(data, content);
    return errors;
}

Errors validate_subspecies_recursively_for_content(const Subspecies::Data& data, const Content& content) {
    Errors errors = validate_subspecies_nonrecursively_for_content(data, content);
    for (const Feature::Data& feature_data : data.features_data) {
        errors += validate_feature_recursively_for_content(feature_data, content);
    }
    return errors;
}

} // namespace dnd
