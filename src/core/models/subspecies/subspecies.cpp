#include <dnd_config.hpp>

#include "subspecies.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <core/content.hpp>
#include <core/data_result.hpp>
#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/source_info.hpp>
#include <core/models/species/species.hpp>
#include <core/validation/subspecies/subspecies_validation.hpp>
#include <core/visitors/content/content_visitor.hpp>

namespace dnd {

CreateResult<Subspecies> Subspecies::create_for(Data&& data, const Content& content) {
    Errors errors = validate_subspecies_nonrecursively_for_content(data, content);
    if (!errors.ok()) {
        return InvalidCreate<Subspecies>(std::move(data), std::move(errors));
    }
    std::vector<Feature> features;
    features.reserve(data.features_data.size());
    for (Feature::Data& feature_data : data.features_data) {
        CreateResult<Feature> feature_result = Feature::create_for(std::move(feature_data), content);
        if (!feature_result.is_valid()) {
            auto [_, sub_errors] = feature_result.data_and_errors();
            return InvalidCreate<Subspecies>(std::move(data), std::move(sub_errors));
        }
        features.push_back(feature_result.value());
    }
    CRef<Species> species = content.get_species_library().get(data.species_key).value();

    return ValidCreate(Subspecies(
        std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(data.source_name),
        std::move(features), species
    ));
}

const std::string& Subspecies::get_name() const { return name; }

const Text& Subspecies::get_description() const { return description; }

const SourceInfo& Subspecies::get_source_info() const { return source_info; }

const std::vector<Feature>& Subspecies::get_features() const { return features; }

CRef<Species> Subspecies::get_species() const { return species; }

void Subspecies::accept_visitor(ContentVisitor& visitor) const { visitor(*this); }

Subspecies::Subspecies(
    std::string&& name, Text&& description, std::filesystem::path&& source_path, std::string&& source_name,
    std::vector<Feature>&& features, CRef<Species> species
)
    : name(std::move(name)), description(std::move(description)),
      source_info({.path = std::move(source_path), .name = std::move(source_name)}), features(std::move(features)),
      species(species) {}

} // namespace dnd
