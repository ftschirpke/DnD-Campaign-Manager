#include <dnd_config.hpp>

#include "character_data.hpp"

#include <array>
#include <vector>

#include <fmt/format.h>

#include <core/basic_mechanics/dice.hpp>
#include <core/controllers/content_holder.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/models/character_class/character_class.hpp>
#include <core/models/character_race/character_race.hpp>
#include <core/validation/character/progression_data.hpp>
#include <core/validation/feature/feature_data.hpp>
#include <core/validation/validation_data.hpp>

dnd::Errors dnd::CharacterData::validate() const {
    Errors errors = ValidationData::validate();
    std::unordered_set<std::string> unique_feature_names;
    for (const auto& feature_data : features_data) {
        errors.merge(feature_data.validate());
        if (unique_feature_names.contains(feature_data.name)) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this,
                fmt::format("Character has duplicate feature \"{}\".", feature_data.name)
            );
        } else {
            unique_feature_names.insert(feature_data.name);
        }
    }
    errors.merge(base_ability_scores_data.validate());
    errors.merge(character_basis_data.validate());
    errors.merge(progression_data.validate());
    return errors;
}

dnd::Errors dnd::CharacterData::validate_relations(const dnd::ContentHolder* content) const {
    Errors errors;
    for (const auto& feature_data : features_data) {
        errors.merge(feature_data.validate_relations(content));
        if (content->features.contains(name)) {
            errors.add_validation_error(
                ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, this,
                fmt::format("Feature has duplicate name \"{}\".", name)
            );
        }
    }

    errors.merge(base_ability_scores_data.validate_relations(content));

    if (!character_basis_data.class_name.empty()
        && content->character_classes.contains(character_basis_data.class_name)) {
        const CharacterClass& cls = content->character_classes.get(character_basis_data.class_name);
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
    return errors;
}
