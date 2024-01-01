#include <dnd_config.hpp>

#include "species_data.hpp"

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include <fmt/format.h>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>

namespace dnd {

static Errors validate_species_raw_nonrecursively(const SpeciesData& data) {
    Errors errors;
    std::unordered_set<std::string> unique_feature_names;
    for (const FeatureData& feature_data : data.features_data) {
        if (unique_feature_names.contains(feature_data.name)) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
                fmt::format("Character species has duplicate feature \"{}\".", feature_data.name)
            );
        } else {
            unique_feature_names.insert(feature_data.name);
        }
    }
    if (data.features_data.empty()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, "Character species has no features."
        );
    }
    return errors;
}

static Errors validate_species_relations_nonrecursively(const SpeciesData& data, const Content& content) {
    Errors errors;
    if (content.get_species().contains(data.name)) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, fmt::format("Species has duplicate name \"{}\".", data.name)
        );
    }
    for (const FeatureData& feature_data : data.features_data) {
        if (content.get_features().contains(feature_data.name)) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE,
                fmt::format("Feature has duplicate name \"{}\".", feature_data.name)
            );
        }
    }
    return errors;
}

Errors validate_species_nonrecursively_for_content(const SpeciesData& data, const Content& content) {
    Errors errors = validate_species_raw_nonrecursively(data);
    errors += validate_species_relations_nonrecursively(data, content);
    return errors;
}

Errors validate_species_recursively_for_content(const SpeciesData& data, const Content& content) {
    Errors errors = validate_species_nonrecursively_for_content(data, content);
    for (const FeatureData& feature_data : data.features_data) {
        errors += validate_feature_recursively_for_content(feature_data, content);
    }
    return errors;
}

} // namespace dnd
