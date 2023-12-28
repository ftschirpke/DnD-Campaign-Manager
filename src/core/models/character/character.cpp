#include <dnd_config.hpp>

#include "character.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <core/basic_mechanics/character_progression.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/character/ability_scores.hpp>
#include <core/models/character/feature_providers.hpp>
#include <core/models/character/progression.hpp>
#include <core/models/class/class.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/source_info.hpp>
#include <core/models/species/species.hpp>
#include <core/models/subclass/subclass.hpp>
#include <core/models/subspecies/subspecies.hpp>
#include <core/validation/character/character_data.hpp>
#include <core/visitors/content/content_visitor.hpp>

namespace dnd {

Character Character::create_for(Data&& data, const Content& content) {
    if (!data.validate().ok()) {
        throw invalid_data("Cannot create character from invalid data.");
    }
    if (!data.validate_relations(content).ok()) {
        throw invalid_data("Character data is incompatible with the given content.");
    }

    std::vector<Feature> features;
    features.reserve(data.features_data.size());
    for (Feature::Data& feature_data : data.features_data) {
        features.emplace_back(Feature::create_for(std::move(feature_data), content));
    }

    AbilityScores base_ability_scores = AbilityScores::create(std::move(data.base_ability_scores_data));
    FeatureProviders feature_providers = FeatureProviders::create_for(std::move(data.feature_providers_data), content);
    Progression progression = Progression::create(std::move(data.progression_data));

    std::vector<Decision> decisions;
    for (Decision::Data& decision_data : data.decisions_data) {
        decisions.emplace_back(Decision::create_for(std::move(decision_data), content));
    }

    return Character(
        std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(features),
        std::move(base_ability_scores), std::move(feature_providers), std::move(progression), std::move(decisions)
    );
}

const std::string& Character::get_name() const noexcept { return name; }

const std::string& Character::get_description() const noexcept { return description; }

const SourceInfo& Character::get_source_info() const noexcept { return source_info; }

const std::vector<Feature>& Character::get_features() const noexcept { return features; }

const std::vector<Choosable>& Character::get_choosables() const noexcept { return choosables; }

const AbilityScores& Character::get_base_ability_scores() const noexcept { return base_ability_scores; }

const FeatureProviders& Character::get_feature_providers() const noexcept { return feature_providers; }

const Progression& Character::get_progression() const noexcept { return progression; }

void Character::for_all_effects_do(std::function<void(const Effects&)> func) const noexcept {
    for (const Feature& feature : feature_providers.get_species().get_features()) {
        func(feature.get_main_effects());
    }
    OptCRef<Subspecies> subspecies = feature_providers.get_subspecies();
    if (subspecies.has_value()) {
        for (const Feature& feature : subspecies.value().get().get_features()) {
            func(feature.get_main_effects());
        }
    }
    for (const ClassFeature& feature : feature_providers.get_class().get_features()) {
        func(feature.get_main_effects());
        for (const auto& [level, effects] : feature.get_higher_level_effects()) {
            if (level > progression.get_level()) {
                break;
            }
            func(effects);
        }
    }
    OptCRef<Subclass> subclass = feature_providers.get_subclass();
    if (subclass.has_value()) {
        for (const ClassFeature& feature : subclass.value().get().get_features()) {
            func(feature.get_main_effects());
            for (const auto& [level, effects] : feature.get_higher_level_effects()) {
                if (level > progression.get_level()) {
                    break;
                }
                func(effects);
            }
        }
    }
    for (const Feature& feature : features) {
        func(feature.get_main_effects());
    }
    for (const Choosable& choosable : choosables) {
        func(choosable.get_main_effects());
    }
}

int Character::get_proficiency_bonus() const noexcept { return proficiency_bonus_for_level(progression.get_level()); }

void Character::accept_visitor(ContentVisitor& visitor) const { visitor(*this); }

Character::Character(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path, std::vector<Feature>&& features,
    AbilityScores&& base_ability_scores, FeatureProviders&& feature_providers, Progression&& progression,
    std::vector<Decision>&& decisions
) noexcept
    : name(std::move(name)), description(std::move(description)), source_info(std::move(source_path)),
      features(std::move(features)), base_ability_scores(std::move(base_ability_scores)),
      feature_providers(std::move(feature_providers)), progression(std::move(progression)),
      decisions(std::move(decisions)) {}

} // namespace dnd
