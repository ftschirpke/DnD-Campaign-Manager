#include <dnd_config.hpp>

#include "character_race.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/source_info.hpp>
#include <core/validation/character_race/character_race_data.hpp>
#include <core/visitors/content/content_visitor.hpp>

dnd::CharacterRace dnd::CharacterRace::create(dnd::CharacterRaceData&& data, const dnd::Content& content) {
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

const std::string& dnd::CharacterRace::get_name() const noexcept { return name; }

const std::string& dnd::CharacterRace::get_description() const noexcept { return description; }

const dnd::SourceInfo& dnd::CharacterRace::get_source_info() const noexcept { return source_info; }

const std::vector<dnd::Feature>& dnd::CharacterRace::get_features() const noexcept { return features; }

bool dnd::CharacterRace::has_subraces() const noexcept { return subraces; }

void dnd::CharacterRace::accept_visitor(dnd::ContentVisitor& visitor) const { visitor(*this); }

dnd::CharacterRace::CharacterRace(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path, std::vector<Feature>&& features,
    bool has_subraces
) noexcept
    : name(std::move(name)), description(std::move(description)), source_info(std::move(source_path)),
      features(std::move(features)), subraces(has_subraces) {}
