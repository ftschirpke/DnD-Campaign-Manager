#include <dnd_config.hpp>

#include "subspecies.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/source_info.hpp>
#include <core/models/species/species.hpp>
#include <core/validation/subspecies/subspecies_data.hpp>
#include <core/visitors/content/content_visitor.hpp>

namespace dnd {

Subspecies Subspecies::create_for(Data&& data, const Content& content) {
    if (!data.validate().ok()) {
        throw invalid_data("Cannot create character subspecies from invalid data.");
    }
    if (!data.validate_relations(content).ok()) {
        throw invalid_data("Character subspecies data is incompatible with the given content.");
    }
    std::vector<Feature> features;
    features.reserve(data.features_data.size());
    for (Feature::Data& feature_data : data.features_data) {
        features.emplace_back(Feature::create(std::move(feature_data), content));
    }
    const Species* species = &content.get_species().get(data.species_name).value().get();
    return Subspecies(
        std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(features), species
    );
}

const std::string& Subspecies::get_name() const noexcept { return name; }

const std::string& Subspecies::get_description() const noexcept { return description; }

const SourceInfo& Subspecies::get_source_info() const noexcept { return source_info; }

const std::vector<Feature>& Subspecies::get_features() const noexcept { return features; }

const Species* Subspecies::get_species() const noexcept { return species; }

void Subspecies::accept_visitor(ContentVisitor& visitor) const { visitor(*this); }

Subspecies::Subspecies(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path, std::vector<Feature>&& features,
    const Species* species
) noexcept
    : name(std::move(name)), description(std::move(description)), source_info(std::move(source_path)),
      features(std::move(features)), species(species) {}

} // namespace dnd
