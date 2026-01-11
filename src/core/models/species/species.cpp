#include <dnd_config.hpp>

#include "species.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/source_info.hpp>
#include <core/text/text.hpp>
#include <core/validation/species/species_validation.hpp>
#include <core/visitors/content/content_visitor.hpp>

namespace dnd {

CreateResult<Species> Species::create_for(Data&& data, const Content& content) {
    Errors errors = validate_species_nonrecursively_for_content(data, content);
    if (!errors.ok()) {
        return InvalidCreate<Species>(std::move(data), std::move(errors));
    }
    std::vector<Feature> features;
    features.reserve(data.features_data.size());
    for (Feature::Data& feature_data : data.features_data) {
        CreateResult<Feature> feature_result = Feature::create_for(std::move(feature_data), content);
        if (!feature_result.is_valid()) {
            auto [_, sub_errors] = feature_result.data_and_errors();
            return InvalidCreate<Species>(std::move(data), std::move(sub_errors));
        }
        features.push_back(feature_result.value());
    }
    return ValidCreate(Species(
        std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(data.source_name),
        data.get_key(), std::move(features)
    ));
}

const std::string& Species::get_name() const { return name; }

const Text& Species::get_description() const { return description; }

const SourceInfo& Species::get_source_info() const { return source_info; }

const std::string& Species::get_key() const { return key; }

const std::vector<Feature>& Species::get_features() const { return features; }

Species::Species(
    std::string&& name, Text&& description, std::filesystem::path&& source_path, std::string&& source_name,
    std::string&& key, std::vector<Feature>&& features
)
    : name(std::move(name)), description(std::move(description)),
      source_info({.path = std::move(source_path), .name = std::move(source_name)}), key(std::move(key)),
      features(std::move(features)) {}

} // namespace dnd
