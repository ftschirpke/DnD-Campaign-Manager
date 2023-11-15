#include <dnd_config.hpp>

#include "character_class.hpp"

#include <algorithm>
#include <cassert>
#include <filesystem>
#include <memory>
#include <regex>
#include <string>
#include <utility>
#include <vector>

#include <core/basic_mechanics/dice.hpp>
#include <core/basic_mechanics/dice_factory.hpp>
#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/character_class/important_levels.hpp>
#include <core/models/effects_provider/class_feature.hpp>
#include <core/models/source_info.hpp>
#include <core/models/spellcasting/spellcasting_factory.hpp>
#include <core/validation/character_class/character_class_data.hpp>
#include <core/validation/effects/condition/condition_data.hpp>
#include <core/validation/effects/effects_data.hpp>
#include <core/validation/effects_provider/feature_data.hpp>
#include <core/visitors/content/content_visitor.hpp>

static constexpr const char* level_activation_regex_cstr = "CLASS_LEVEL >= [123456789]\\d?";

static int determine_subclass_level(const dnd::FeatureData& subclass_feature_data) {
    static const std::regex level_activation_regex(level_activation_regex_cstr);
    const std::vector<dnd::ConditionData>& activation_conditions = subclass_feature_data.main_effects_data
                                                                       .activation_conditions_data;
    for (const dnd::ConditionData& condition_data : activation_conditions) {
        if (std::regex_match(condition_data.condition_str, level_activation_regex)) {
            return std::stoi(condition_data.condition_str.substr(15));
        }
    }
    return 1;
}

dnd::CharacterClass dnd::CharacterClass::create(dnd::CharacterClassData&& data, const dnd::Content& content) {
    if (!data.validate().ok()) {
        throw invalid_data("Cannot create character class from invalid data.");
    }
    if (!data.validate_relations(content).ok()) {
        throw invalid_data("Character class data is incompatible with the given content.");
    }

    int subclass_level = -1;
    const ClassFeature* subclass_feature = nullptr;

    std::vector<ClassFeature> features;
    features.reserve(data.features_data.size());
    for (auto& feature_data : data.features_data) {
        if (feature_data.name == data.subclass_feature_name) {
            subclass_level = determine_subclass_level(feature_data);
            features.emplace_back(ClassFeature::create(std::move(feature_data), content));
            subclass_feature = &features.back();
        } else {
            features.emplace_back(ClassFeature::create(std::move(feature_data), content));
        }
    }
    assert(subclass_level != -1);
    assert(subclass_feature != nullptr);

    Dice hit_dice = create_dice(std::move(data.hit_dice_data));
    ImportantLevels important_levels = ImportantLevels::create(std::move(data.important_levels_data), subclass_level);

    return CharacterClass(
        std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(features),
        subclass_feature, std::move(hit_dice), std::move(important_levels),
        create_spellcasting(std::move(data.spellcasting_data))
    );
}

const std::string& dnd::CharacterClass::get_name() const noexcept { return name; }

const std::string& dnd::CharacterClass::get_description() const noexcept { return description; }

const dnd::SourceInfo& dnd::CharacterClass::get_source_info() const noexcept { return source_info; }

const std::vector<dnd::ClassFeature>& dnd::CharacterClass::get_features() const noexcept { return features; }

bool dnd::CharacterClass::has_spellcasting() const noexcept { return spellcasting != nullptr; }

const dnd::Spellcasting* dnd::CharacterClass::get_spellcasting() const noexcept { return spellcasting.get(); }

const dnd::ClassFeature* dnd::CharacterClass::get_subclass_feature() const noexcept { return subclass_feature; }

const dnd::Dice& dnd::CharacterClass::get_hit_dice() const noexcept { return hit_dice; }

const dnd::ImportantLevels& dnd::CharacterClass::get_important_levels() const noexcept { return important_levels; }

void dnd::CharacterClass::accept(dnd::ContentVisitor& visitor) const { visitor(*this); }

dnd::CharacterClass::CharacterClass(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path,
    std::vector<dnd::ClassFeature>&& features, const dnd::ClassFeature* subclass_feature, dnd::Dice hit_dice,
    dnd::ImportantLevels&& important_levels, std::unique_ptr<dnd::Spellcasting>&& spellcasting
) noexcept
    : name(std::move(name)), description(std::move(description)), source_info(std::move(source_path)),
      features(std::move(features)), spellcasting(std::move(spellcasting)), subclass_feature(subclass_feature),
      hit_dice(std::move(hit_dice)), important_levels(std::move(important_levels)) {}
