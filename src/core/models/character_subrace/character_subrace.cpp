#include <dnd_config.hpp>

#include "character_subrace.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <core/content_visitors/content_visitor.hpp>
#include <core/controllers/content_holder.hpp>
#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/character_race/character_race.hpp>
#include <core/validation/character_subrace/character_subrace_data.hpp>

dnd::CharacterSubrace dnd::CharacterSubrace::create(
    dnd::CharacterSubraceData&& data, const dnd::ContentHolder* content
) {
    if (!data.validate().ok()) {
        throw invalid_data("Cannot create character subrace from invalid data.");
    }
    if (!data.validate_relations(content).ok()) {
        throw invalid_data("Character subrace data is incompatible with the given content.");
    }
    std::vector<Feature> features;
    features.reserve(data.features_data.size());
    for (auto& feature_data : data.features_data) {
        features.emplace_back(Feature::create(std::move(feature_data), content));
    }
    const CharacterRace* race = &content->character_races.get(data.race_name);
    return CharacterSubrace(
        std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(features), race
    );
}

const dnd::CharacterRace* dnd::CharacterSubrace::get_race() const noexcept { return race; }
