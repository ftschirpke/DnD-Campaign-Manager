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

std::unique_ptr<ValidationData> SubspeciesData::pack() const {
    return std::make_unique<SubspeciesData>(*this);
}

Errors SubspeciesData::validate() const {
    Errors errors;
    std::unordered_set<std::string> unique_feature_names;
    for (const FeatureData& feature_data : features_data) {
        errors += feature_data.validate();
        if (unique_feature_names.contains(feature_data.name)) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this,
                fmt::format("Character class has duplicate feature \"{}\".", feature_data.name)
            );
        } else {
            unique_feature_names.insert(feature_data.name);
        }
    }
    if (features_data.empty()) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, "Character subspecies has no features."
        );
    }
    if (species_name.empty()) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, "Character subspecies has no species name."
        );
    }
    return errors;
}

Errors SubspeciesData::validate_relations(const Content& content) const {
    Errors errors;
    if (content.get_subspecies().contains(name)) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this,
            fmt::format("Subspecies has duplicate name \"{}\".", name)
        );
    }
    for (const FeatureData& feature_data : features_data) {
        errors += feature_data.validate_relations(content);
        if (content.get_features().contains(feature_data.name)) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this,
                fmt::format("Feature has duplicate name \"{}\".", feature_data.name)
            );
        }
    }
    OptCRef<Species> species_optional = content.get_species().get(species_name);
    if (!species_optional.has_value()) {
        errors.add_validation_error(
            ValidationErrorCode::RELATION_NOT_FOUND, this,
            fmt::format("Character species '{}' does not exist.", species_name)
        );
    } else if (!species_optional.value().get().has_subspecies()) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_RELATION, this,
            fmt::format("Character species '{}' does not have subspecies.", species_name)
        );
    }
    return errors;
}

} // namespace dnd
