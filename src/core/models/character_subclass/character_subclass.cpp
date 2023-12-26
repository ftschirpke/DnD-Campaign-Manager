#include <dnd_config.hpp>

#include "character_subclass.hpp"

#include <filesystem>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/character_species/character_species.hpp>
#include <core/models/effects_provider/class_feature.hpp>
#include <core/models/source_info.hpp>
#include <core/models/spellcasting/spellcasting_factory.hpp>
#include <core/validation/character_subclass/character_subclass_data.hpp>
#include <core/visitors/content/content_visitor.hpp>

dnd::CharacterSubclass dnd::CharacterSubclass::create(dnd::CharacterSubclassData&& data, const dnd::Content& content) {
    if (!data.validate().ok()) {
        throw invalid_data("Cannot create character subclass from invalid data.");
    }
    if (!data.validate_relations(content).ok()) {
        throw invalid_data("Character subclass data is incompatible with the given content.");
    }
    std::vector<ClassFeature> features;
    features.reserve(data.features_data.size());
    for (ClassFeatureData& feature_data : data.features_data) {
        features.emplace_back(ClassFeature::create(std::move(feature_data), content));
    }
    const CharacterClass* cls = &content.get_character_classes().get(data.class_name).value().get();
    return CharacterSubclass(
        std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(features), cls,
        create_spellcasting(std::move(data.spellcasting_data))
    );
}

const std::string& dnd::CharacterSubclass::get_name() const noexcept { return name; }

const std::string& dnd::CharacterSubclass::get_description() const noexcept { return description; }

const dnd::SourceInfo& dnd::CharacterSubclass::get_source_info() const noexcept { return source_info; }

const std::vector<dnd::ClassFeature>& dnd::CharacterSubclass::get_features() const noexcept { return features; }

bool dnd::CharacterSubclass::has_spellcasting() const noexcept { return spellcasting != nullptr; }

const dnd::Spellcasting* dnd::CharacterSubclass::get_spellcasting() const noexcept { return spellcasting.get(); }

const dnd::CharacterClass* dnd::CharacterSubclass::get_class() const noexcept { return cls; }

void dnd::CharacterSubclass::accept_visitor(dnd::ContentVisitor& visitor) const { visitor(*this); }

dnd::CharacterSubclass::CharacterSubclass(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path,
    std::vector<ClassFeature>&& features, const CharacterClass* cls, std::unique_ptr<Spellcasting>&& spellcasting
) noexcept
    : name(std::move(name)), description(std::move(description)), source_info(std::move(source_path)),
      features(std::move(features)), cls(cls), spellcasting(std::move(spellcasting)) {}
