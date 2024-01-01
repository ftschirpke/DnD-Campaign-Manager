#include <dnd_config.hpp>

#include "subspecies_data.hpp"

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include <fmt/format.h>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/validation/effects_provider/feature_data.hpp>

namespace dnd {

std::unique_ptr<ValidationData> SubspeciesData::pack() const { return std::make_unique<SubspeciesData>(*this); }

static Errors validate_subspecies_raw_nonrecursively(const SubspeciesData& data) {
    Errors errors;
    std::unordered_set<std::string> unique_feature_names;
    for (const FeatureData& feature_data : data.features_data) {
        if (unique_feature_names.contains(feature_data.name)) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.pack(),
                fmt::format("Character class has duplicate feature \"{}\".", feature_data.name)
            );
        } else {
            unique_feature_names.insert(feature_data.name);
        }
    }
    if (data.features_data.empty()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.pack(), "Character subspecies has no features."
        );
    }
    if (data.species_name.empty()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.pack(), "Character subspecies has no species name."
        );
    }
    return errors;
}

Errors validate_subspecies_relations_nonrecursively(const SubspeciesData& data, const Content& content) {
    Errors errors;
    if (content.get_subspecies().contains(data.name)) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.pack(),
            fmt::format("Subspecies has duplicate name \"{}\".", data.name)
        );
    }
    for (const FeatureData& feature_data : data.features_data) {
        if (content.get_features().contains(feature_data.name)) {
            errors.add_validation_error(
                ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.pack(),
                fmt::format("Feature has duplicate name \"{}\".", feature_data.name)
            );
        }
    }
    OptCRef<Species> species_optional = content.get_species().get(data.species_name);
    if (!species_optional.has_value()) {
        errors.add_validation_error(
            ValidationError::Code::RELATION_NOT_FOUND, data.pack(),
            fmt::format("Character species '{}' does not exist.", data.species_name)
        );
    } else if (!species_optional.value().get().has_subspecies()) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_RELATION, data.pack(),
            fmt::format("Character species '{}' does not have subspecies.", data.species_name)
        );
    }
    return errors;
}

Errors validate_subspecies_nonrecursively_for_content(const SubspeciesData& data, const Content& content) {
    Errors errors = validate_subspecies_raw_nonrecursively(data);
    errors += validate_subspecies_relations_nonrecursively(data, content);
    return errors;
}

Errors validate_subspecies_recursively_for_content(const SubspeciesData& data, const Content& content) {
    Errors errors = validate_subspecies_nonrecursively_for_content(data, content);
    for (const FeatureData& feature_data : data.features_data) {
        errors += validate_feature_recursively_for_content(feature_data, content);
    }
    return errors;
}

} // namespace dnd
