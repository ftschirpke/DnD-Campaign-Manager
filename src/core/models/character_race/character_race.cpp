#include <dnd_config.hpp>

#include "character_race.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <core/content_visitors/content_visitor.hpp>
#include <core/controllers/content_holder.hpp>
#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/validation/character_race/character_race_data.hpp>

dnd::CharacterRace dnd::CharacterRace::create(dnd::CharacterRaceData&& data, const dnd::ContentHolder& content) {
    if (!data.validate().ok()) {
        throw invalid_data("Cannot create character race from invalid data.");
    }
    if (!data.validate_relations(content).ok()) {
        throw invalid_data("Character race data is incompatible with the given content.");
    }
    std::vector<Feature> features;
    features.reserve(data.features_data.size());
    for (auto& feature_data : data.features_data) {
        features.emplace_back(Feature::create(std::move(feature_data), content));
    }
    return CharacterRace(
        std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(features),
        data.subraces
    );
}

bool dnd::CharacterRace::has_subraces() const noexcept { return subraces; }

void dnd::CharacterRace::accept(dnd::ContentVisitor* visitor) const { visitor->visit(this); }

dnd::CharacterRace::CharacterRace(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path, std::vector<Feature>&& features,
    bool has_subraces
) noexcept
    : FeatureHolder(std::move(name), std::move(description), std::move(source_path), std::move(features)),
      subraces(has_subraces) {}
