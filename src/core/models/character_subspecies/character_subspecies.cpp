#include <dnd_config.hpp>

#include "character_subspecies.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/character_species/character_species.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/source_info.hpp>
#include <core/validation/character_subspecies/character_subspecies_data.hpp>
#include <core/visitors/content/content_visitor.hpp>

dnd::CharacterSubspecies dnd::CharacterSubspecies::create(dnd::CharacterSubspeciesData&& data, const dnd::Content& content) {
    if (!data.validate().ok()) {
        throw invalid_data("Cannot create character subspecies from invalid data.");
    }
    if (!data.validate_relations(content).ok()) {
        throw invalid_data("Character subspecies data is incompatible with the given content.");
    }
    std::vector<Feature> features;
    features.reserve(data.features_data.size());
    for (FeatureData& feature_data : data.features_data) {
        features.emplace_back(Feature::create(std::move(feature_data), content));
    }
    const CharacterSpecies* species = &content.get_character_species().get(data.species_name).value().get();
    return CharacterSubspecies(
        std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(features), species
    );
}

const std::string& dnd::CharacterSubspecies::get_name() const noexcept { return name; }

const std::string& dnd::CharacterSubspecies::get_description() const noexcept { return description; }

const dnd::SourceInfo& dnd::CharacterSubspecies::get_source_info() const noexcept { return source_info; }

const std::vector<dnd::Feature>& dnd::CharacterSubspecies::get_features() const noexcept { return features; }

const dnd::CharacterSpecies* dnd::CharacterSubspecies::get_species() const noexcept { return species; }

void dnd::CharacterSubspecies::accept_visitor(dnd::ContentVisitor& visitor) const { visitor(*this); }

dnd::CharacterSubspecies::CharacterSubspecies(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path, std::vector<Feature>&& features,
    const CharacterSpecies* species
) noexcept
    : name(std::move(name)), description(std::move(description)), source_info(std::move(source_path)),
      features(std::move(features)), species(species) {}
