#include <dnd_config.hpp>

#include "character_species_data.hpp"

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include <fmt/format.h>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>

std::unique_ptr<dnd::ValidationData> dnd::CharacterSpeciesData::pack() const {
    return std::make_unique<CharacterSpeciesData>(*this);
}

dnd::Errors dnd::CharacterSpeciesData::validate() const {
    Errors errors;
    std::unordered_set<std::string> unique_feature_names;
    for (const FeatureData& feature_data : features_data) {
        errors += feature_data.validate();
        if (unique_feature_names.contains(feature_data.name)) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this,
                fmt::format("Character species has duplicate feature \"{}\".", feature_data.name)
            );
        } else {
            unique_feature_names.insert(feature_data.name);
        }
    }
    if (features_data.empty()) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, "Character species has no features."
        );
    }
    return errors;
}

dnd::Errors dnd::CharacterSpeciesData::validate_relations(const Content& content) const {
    Errors errors;
    if (content.get_character_species().contains(name)) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this, fmt::format("Species has duplicate name \"{}\".", name)
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
    return errors;
}
