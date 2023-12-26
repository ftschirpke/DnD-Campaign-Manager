#include <dnd_config.hpp>

#include "character_species.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/source_info.hpp>
#include <core/validation/character_species/character_species_data.hpp>
#include <core/visitors/content/content_visitor.hpp>

dnd::CharacterSpecies dnd::CharacterSpecies::create(dnd::CharacterSpeciesData&& data, const dnd::Content& content) {
    if (!data.validate().ok()) {
        throw invalid_data("Cannot create character species from invalid data.");
    }
    if (!data.validate_relations(content).ok()) {
        throw invalid_data("Character species data is incompatible with the given content.");
    }
    std::vector<Feature> features;
    features.reserve(data.features_data.size());
    for (FeatureData& feature_data : data.features_data) {
        features.emplace_back(Feature::create(std::move(feature_data), content));
    }
    return CharacterSpecies(
        std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(features),
        data.subspecies
    );
}

const std::string& dnd::CharacterSpecies::get_name() const noexcept { return name; }

const std::string& dnd::CharacterSpecies::get_description() const noexcept { return description; }

const dnd::SourceInfo& dnd::CharacterSpecies::get_source_info() const noexcept { return source_info; }

const std::vector<dnd::Feature>& dnd::CharacterSpecies::get_features() const noexcept { return features; }

bool dnd::CharacterSpecies::has_subspecies() const noexcept { return subspecies; }

void dnd::CharacterSpecies::accept_visitor(dnd::ContentVisitor& visitor) const { visitor(*this); }

dnd::CharacterSpecies::CharacterSpecies(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path, std::vector<Feature>&& features,
    bool has_subspecies
) noexcept
    : name(std::move(name)), description(std::move(description)), source_info(std::move(source_path)),
      features(std::move(features)), subspecies(has_subspecies) {}
