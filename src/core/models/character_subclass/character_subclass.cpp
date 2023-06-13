#include <dnd_config.hpp>

#include "character_subclass.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <core/content_visitors/content_visitor.hpp>
#include <core/controllers/content_holder.hpp>
#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/character_race/character_race.hpp>
#include <core/validation/character_subclass/character_subclass_data.hpp>

dnd::CharacterSubclass dnd::CharacterSubclass::create(
    dnd::CharacterSubclassData&& data, const dnd::ContentHolder* content
) {
    if (!data.validate().ok()) {
        throw invalid_data("Cannot create character subclass from invalid data.");
    }
    if (!data.validate_relations(content).ok()) {
        throw invalid_data("Character subclass data is incompatible with the given content.");
    }
    std::vector<Feature> features;
    features.reserve(data.features_data.size());
    for (auto& feature_data : data.features_data) {
        features.emplace_back(Feature::create(std::move(feature_data), content));
    }
    const CharacterClass* cls = &content->character_classes.get(data.class_name);
    return CharacterSubclass(
        std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(features), cls
    );
}

const dnd::CharacterClass* dnd::CharacterSubclass::get_class() const noexcept { return cls; }
