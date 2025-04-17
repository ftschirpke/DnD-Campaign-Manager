#include <dnd_config.hpp>

#include "species_validation.hpp"

#include <fmt/format.h>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/species/species.hpp>

namespace dnd {

static Errors validate_species_raw_nonrecursively(const Species::Data& data) {
    Errors errors;
    DND_UNUSED(data);
    // TODO: re-evaluate whether species without features should be allowed
    return errors;
}

static Errors validate_species_relations_nonrecursively(const Species::Data& data, const Content& content) {
    Errors errors;
    if (content.get_species().contains(data.get_key())) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
            fmt::format("Species has duplicate key \"{}\".", data.get_key())
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
    return errors;
}

Errors validate_species_nonrecursively_for_content(const Species::Data& data, const Content& content) {
    Errors errors = validate_species_raw_nonrecursively(data);
    errors += validate_species_relations_nonrecursively(data, content);
    return errors;
}

Errors validate_species_recursively_for_content(const Species::Data& data, const Content& content) {
    Errors errors = validate_species_nonrecursively_for_content(data, content);
    for (const Feature::Data& feature_data : data.features_data) {
        errors += validate_feature_recursively_for_content(feature_data, content);
    }
    return errors;
}

} // namespace dnd
