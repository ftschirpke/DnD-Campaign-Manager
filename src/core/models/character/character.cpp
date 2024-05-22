#include <dnd_config.hpp>

#include "character.hpp"

#include <cassert>
#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <tl/expected.hpp>

#include <core/basic_mechanics/character_progression.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/character/ability_scores.hpp>
#include <core/models/character/feature_providers.hpp>
#include <core/models/character/progression.hpp>
#include <core/models/character/stats.hpp>
#include <core/models/class/class.hpp>
#include <core/models/effects/stat_change/identifier_stat_change.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/source_info.hpp>
#include <core/models/species/species.hpp>
#include <core/models/subclass/subclass.hpp>
#include <core/models/subspecies/subspecies.hpp>
#include <core/utils/data_result.hpp>
#include <core/validation/character/character_validation.hpp>
#include <core/visitors/content/content_visitor.hpp>

namespace dnd {

CreateResult<Character> Character::create_for(Data&& data, const Content& content) {
    Errors errors = validate_character_nonrecursively_for_content(data, content);
    if (!errors.ok()) {
        return InvalidCreate<Character>(std::move(data), std::move(errors));
    }

    std::vector<Feature> features;
    features.reserve(data.features_data.size());
    for (Feature::Data& feature_data : data.features_data) {
        CreateResult<Feature> feature_result = Feature::create_for(std::move(feature_data), content);
        if (!feature_result.is_valid()) {
            auto [_, sub_errors] = feature_result.data_and_errors();
            return InvalidCreate<Character>(std::move(data), std::move(sub_errors));
        }
        features.push_back(feature_result.value());
    }

    CreateResult<AbilityScores> base_ability_scores_result = AbilityScores::create(
        std::move(data.base_ability_scores_data)
    );
    if (!base_ability_scores_result.is_valid()) {
        auto [_, sub_errors] = base_ability_scores_result.data_and_errors();
        return InvalidCreate<Character>(std::move(data), std::move(sub_errors));
    }
    AbilityScores base_ability_scores = base_ability_scores_result.value();

    CreateResult<FeatureProviders> feature_providers_result = FeatureProviders::create_for(
        std::move(data.feature_providers_data), content
    );
    if (!feature_providers_result.is_valid()) {
        auto [_, sub_errors] = feature_providers_result.data_and_errors();
        return InvalidCreate<Character>(std::move(data), std::move(sub_errors));
    }
    FeatureProviders feature_providers = feature_providers_result.value();

    CreateResult<Progression> progression_result = Progression::create(std::move(data.progression_data));
    if (!progression_result.is_valid()) {
        auto [_, sub_errors] = progression_result.data_and_errors();
        return InvalidCreate<Character>(std::move(data), std::move(sub_errors));
    }
    Progression progression = progression_result.value();

    std::vector<Decision> decisions;
    for (Decision::Data& decision_data : data.decisions_data) {
        CreateResult<Decision> decision_result = Decision::create_for(std::move(decision_data), content);
        if (!decision_result.is_valid()) {
            auto [_, sub_errors] = decision_result.data_and_errors();
            return InvalidCreate<Character>(std::move(data), std::move(sub_errors));
        }
        decisions.push_back(decision_result.value());
    }

    Character character(
        std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(features),
        std::move(base_ability_scores), std::move(feature_providers), std::move(progression), std::move(decisions)
    );
    errors = character.recalculate_stats();
    if (!errors.ok()) {
        return InvalidCreate<Character>(std::move(data), std::move(errors));
    }
    return ValidCreate(std::move(character));
}

const std::string& Character::get_name() const { return name; }

const std::string& Character::get_description() const { return description; }

const SourceInfo& Character::get_source_info() const { return source_info; }

const std::vector<Feature>& Character::get_features() const { return features; }

const std::vector<Choosable>& Character::get_choosables() const { return choosables; }

const AbilityScores& Character::get_base_ability_scores() const { return base_ability_scores; }

const FeatureProviders& Character::get_feature_providers() const { return feature_providers; }

const Progression& Character::get_progression() const { return progression; }

const Stats& Character::get_stats() const { return stats; }

void Character::for_all_effects_do(std::function<void(const Effects&)> func) const {
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

Errors Character::recalculate_stats() {
    std::vector<CRef<StatChange>> stat_changes;
    std::vector<IdentifierStatChange> implicit_stat_changes;
    for_all_effects_do([&stat_changes, &implicit_stat_changes](const Effects& effects) {
        for (const std::unique_ptr<StatChange>& change : effects.get_stat_changes()) {
            stat_changes.push_back(*change);
        }
        std::vector<std::string> proficient_skills = effects.get_proficiencies().get_skill_proficiencies();
        for (const std::string& proficient_skill : proficient_skills) {
            const IdentifierStatChange& change = implicit_stat_changes.emplace_back(
                proficient_skill, StatChangeTime::NORMAL, StatChangeOperation::ADD, "PB"
            );
            stat_changes.push_back(change);
        }
        std::vector<std::string> proficient_saves = effects.get_proficiencies().get_saving_throw_proficiencies();
        for (const std::string& proficient_save : proficient_saves) {
            const IdentifierStatChange& change = implicit_stat_changes.emplace_back(
                proficient_save, StatChangeTime::NORMAL, StatChangeOperation::ADD, "PB"
            );
            stat_changes.push_back(change);
        }
    });

    tl::expected<Stats, Errors> result = Stats::create(get_proficiency_bonus(), base_ability_scores, stat_changes);
    if (!result.has_value()) {
        return result.error();
    }
    stats = result.value();

    return Errors();
}

int Character::get_proficiency_bonus() const {
    tl::expected<int, RuntimeError> proficiency_bonus_result = proficiency_bonus_for_level(progression.get_level());
    assert(proficiency_bonus_result.has_value());
    return proficiency_bonus_result.value();
}

void Character::accept_visitor(ContentVisitor& visitor) const { visitor(*this); }

Character::Character(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path, std::vector<Feature>&& features,
    AbilityScores&& base_ability_scores, FeatureProviders&& feature_providers, Progression&& progression,
    std::vector<Decision>&& decisions
)
    : name(std::move(name)), description(std::move(description)), source_info(std::move(source_path)),
      features(std::move(features)), base_ability_scores(std::move(base_ability_scores)),
      feature_providers(std::move(feature_providers)), progression(std::move(progression)),
      stats(Stats::create_default()), decisions(std::move(decisions)) {}

} // namespace dnd
