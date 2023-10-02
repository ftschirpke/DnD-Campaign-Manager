#include <dnd_config.hpp>

#include "character_data.hpp"

#include <array>
#include <memory>
#include <unordered_set>
#include <vector>

#include <fmt/format.h>

#include <core/basic_mechanics/dice.hpp>
#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/models/character_class/character_class.hpp>
#include <core/models/character_race/character_race.hpp>
#include <core/models/effect_holder/effect_holder.hpp>
#include <core/models/feature/feature.hpp>
#include <core/referencing_content_library.hpp>
#include <core/validation/character/progression_data.hpp>
#include <core/validation/feature/feature_data.hpp>
#include <core/validation/validation_data.hpp>

dnd::CharacterData::CharacterData() noexcept
    : ValidationData(), features_data(), base_ability_scores_data(this), character_basis_data(this),
      progression_data(this), decisions_data() {}

std::unique_ptr<dnd::ValidationData> dnd::CharacterData::pack() const { return std::make_unique<CharacterData>(*this); }

dnd::Errors dnd::CharacterData::validate() const {
    Errors errors = ValidationData::validate();
    std::unordered_set<std::string> unique_feature_names;
    for (const auto& feature_data : features_data) {
        errors += feature_data.validate();
        if (unique_feature_names.contains(feature_data.name)) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this,
                fmt::format("Character has duplicate feature \"{}\".", feature_data.name)
            );
        } else {
            unique_feature_names.insert(feature_data.name);
        }
    }
    errors += base_ability_scores_data.validate();
    errors += character_basis_data.validate();
    errors += progression_data.validate();
    for (const auto& decision_data : decisions_data) {
        errors += decision_data.validate();
    }
    return errors;
}

std::vector<const dnd::FeatureHolder*> get_feature_holders(
    const dnd::CharacterBasisData& basis, const dnd::Content& content
) {
    std::vector<const dnd::FeatureHolder*> feature_holders;
    if (content.get_character_races().contains(basis.race_name)) {
        feature_holders.push_back(&content.get_character_races().get(basis.race_name));
    }
    if (content.get_character_subraces().contains(basis.subrace_name)) {
        feature_holders.push_back(&content.get_character_subraces().get(basis.subrace_name));
    }
    if (content.get_character_classes().contains(basis.class_name)) {
        feature_holders.push_back(&content.get_character_classes().get(basis.class_name));
    }
    if (content.get_character_subclasses().contains(basis.subclass_name)) {
        feature_holders.push_back(&content.get_character_subclasses().get(basis.subclass_name));
    }
    return feature_holders;
}

dnd::Errors dnd::CharacterData::validate_relations(const dnd::Content& content) const {
    Errors errors;
    if (content.get_characters().contains(name)) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this,
            fmt::format("Character has duplicate name \"{}\".", name)
        );
    }
    for (const auto& feature_data : features_data) {
        errors += feature_data.validate_relations(content);
        if (content.get_features().contains(feature_data.name)) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this,
                fmt::format("Feature has duplicate name \"{}\".", feature_data.name)
            );
        }
    }

    errors += base_ability_scores_data.validate_relations(content);
    errors += character_basis_data.validate_relations(content);

    if (!character_basis_data.class_name.empty()
        && content.get_character_classes().contains(character_basis_data.class_name)) {
        const CharacterClass& cls = content.get_character_classes().get(character_basis_data.class_name);
        if (progression_data.level >= cls.get_important_levels().get_subclass_level()
            && character_basis_data.subclass_name.empty()) {
            errors.add_validation_error(
                ValidationErrorCode::INCONSISTENT_ATTRIBUTES, this,
                fmt::format(
                    "{}s of level {} must have a subclass and {} is level {}.", cls.get_name(),
                    cls.get_important_levels().get_subclass_level(), name, progression_data.level
                )
            );
        } else if (progression_data.level < cls.get_important_levels().get_subclass_level() && !character_basis_data.subclass_name.empty()) {
            errors.add_validation_error(
                ValidationErrorCode::INCONSISTENT_ATTRIBUTES, this,
                fmt::format(
                    "{}s cannot have a subclass before level {} and {} is only level {}.", cls.get_name(),
                    cls.get_important_levels().get_subclass_level(), name, progression_data.level
                )
            );
        }

        for (auto& hit_dice_roll : progression_data.hit_dice_rolls) {
            if (!value_is_possible_for(hit_dice_roll, cls.get_hit_dice())) {
                errors.add_validation_error(
                    ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this,
                    fmt::format(
                        "Character has invalid hit dice roll of {} (cannot be larger than {}).", hit_dice_roll,
                        dice_to_int(cls.get_hit_dice())
                    )
                );
            }
        }
    }

    for (const auto& decision_data : decisions_data) {
        errors += decision_data.validate_relations(content);
    }

    std::vector<const EffectHolder*> effect_holders_with_choices;
    for (const auto& [_, feature] : content.get_features().get_all()) {
        std::vector<const EffectHolder*> effect_holders = {&feature->get_main_part()};
        for (const EffectHolder& effect_holder : feature->get_other_parts()) {
            effect_holders.push_back(&effect_holder);
        }
        for (const EffectHolder* effect_holder : effect_holders) {
            if (!effect_holder->get_choices().empty()) {
                effect_holders_with_choices.push_back(effect_holder);
            }
        }
    }

    return errors;
}
