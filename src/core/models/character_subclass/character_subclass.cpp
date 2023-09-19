#include <dnd_config.hpp>

#include "character_subclass.hpp"

#include <filesystem>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <core/content.hpp>
#include <core/content_visitors/content_visitor.hpp>
#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/character_class/spellcasting/spellcasting_factory.hpp>
#include <core/models/character_race/character_race.hpp>
#include <core/validation/character_subclass/character_subclass_data.hpp>

dnd::CharacterSubclass dnd::CharacterSubclass::create(dnd::CharacterSubclassData&& data, const dnd::Content& content) {
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
    const CharacterClass* cls = &content.get_character_classes().get(data.class_name);
    return CharacterSubclass(
        std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(features), cls,
        create_spellcasting(std::move(data.spellcasting_data))
    );
}

bool dnd::CharacterSubclass::has_spellcasting() const noexcept { return spellcasting != nullptr; }

const dnd::CharacterClass* dnd::CharacterSubclass::get_class() const noexcept { return cls; }

void dnd::CharacterSubclass::accept(dnd::ContentVisitor* visitor) const { visitor->visit(this); }

dnd::CharacterSubclass::CharacterSubclass(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path, std::vector<Feature>&& features,
    const CharacterClass* cls, std::unique_ptr<Spellcasting>&& spellcasting
) noexcept
    : FeatureHolder(std::move(name), std::move(description), std::move(source_path), std::move(features)), cls(cls),
      spellcasting(std::move(spellcasting)) {}
