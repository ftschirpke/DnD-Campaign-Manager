#include <dnd_config.hpp>

#include "character.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/character/ability_scores.hpp>
#include <core/models/character/character_basis.hpp>
#include <core/models/character/progression.hpp>
#include <core/models/character_class/character_class.hpp>
#include <core/models/character_race/character_race.hpp>
#include <core/models/character_subclass/character_subclass.hpp>
#include <core/models/character_subrace/character_subrace.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/source_info.hpp>
#include <core/validation/character/character_data.hpp>
#include <core/visitors/content/content_visitor.hpp>

dnd::Character dnd::Character::create(dnd::CharacterData&& data, const Content& content) {
    if (!data.validate().ok()) {
        throw dnd::invalid_data("Cannot create character from invalid data.");
    }
    if (!data.validate_relations(content).ok()) {
        throw dnd::invalid_data("Character data is incompatible with the given content.");
    }

    std::vector<Feature> features;
    features.reserve(data.features_data.size());
    for (auto& feature_data : data.features_data) {
        features.emplace_back(Feature::create(std::move(feature_data), content));
    }

    AbilityScores base_ability_scores = AbilityScores::create(std::move(data.base_ability_scores_data));
    CharacterBasis basis = CharacterBasis::create(std::move(data.character_basis_data), content);
    Progression progression = Progression::create(std::move(data.progression_data));

    std::vector<Decision> decisions;
    for (auto& decision_data : data.decisions_data) {
        decisions.emplace_back(Decision::create(std::move(decision_data), content));
    }

    return Character(
        std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(features),
        std::move(base_ability_scores), std::move(basis), std::move(progression), std::move(decisions)
    );
}

const std::string& dnd::Character::get_name() const noexcept { return name; }

const std::string& dnd::Character::get_description() const noexcept { return description; }

const dnd::SourceInfo& dnd::Character::get_source_info() const noexcept { return source_info; }

const std::vector<dnd::Feature>& dnd::Character::get_features() const noexcept { return features; }

const dnd::AbilityScores& dnd::Character::get_base_ability_scores() const noexcept { return base_ability_scores; }

const dnd::CharacterBasis& dnd::Character::get_basis() const noexcept { return basis; }

const dnd::Progression& dnd::Character::get_progression() const noexcept { return progression; }

void dnd::Character::accept_visitor(dnd::ContentVisitor& visitor) const { visitor(*this); }

dnd::Character::Character(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path,
    std::vector<dnd::Feature>&& features, dnd::AbilityScores&& base_ability_scores, dnd::CharacterBasis&& basis,
    dnd::Progression&& progression, std::vector<dnd::Decision>&& decisions
) noexcept
    : name(std::move(name)), description(std::move(description)), source_info(std::move(source_path)),
      features(std::move(features)), base_ability_scores(std::move(base_ability_scores)), basis(std::move(basis)),
      progression(std::move(progression)), decisions(std::move(decisions)) {}
