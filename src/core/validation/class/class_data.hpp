#ifndef CHARACTER_CLASS_DATA_HPP_
#define CHARACTER_CLASS_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <memory>
#include <string>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/validation/basic_mechanics/dice_data.hpp>
#include <core/validation/character_class/important_levels_data.hpp>
#include <core/validation/effects_provider/class_feature_data.hpp>
#include <core/validation/spellcasting/spellcasting_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class Content;

class CharacterClassData : public ValidationData {
public:
    CharacterClassData() noexcept;
    std::strong_ordering operator<=>(const CharacterClassData&) const noexcept = default;
    virtual std::unique_ptr<ValidationData> pack() const override;
    virtual Errors validate() const override;
    virtual Errors validate_relations(const Content& content) const override;

    SpellcastingData spellcasting_data;
    std::vector<ClassFeatureData> features_data;
    std::string subclass_feature_name;
    DiceData hit_dice_data;
    ImportantLevelsData important_levels_data;
};

} // namespace dnd

#endif // CHARACTER_CLASS_DATA_HPP_
