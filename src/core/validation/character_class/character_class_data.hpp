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
#include <core/validation/effects_provider/feature_data.hpp>
#include <core/validation/spellcasting/spellcasting_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class Content;

class CharacterClassData : public ValidationData {
public:
    CharacterClassData() noexcept;
    std::strong_ordering operator<=>(const CharacterClassData&) const noexcept = default;
    /**
     * @brief Packs the data into a ValidationData unique pointer
     * @return the packed data
     */
    virtual std::unique_ptr<ValidationData> pack() const override;
    /**
     * @brief Validates the data
     * @return the errors that occured during validation
     */
    virtual Errors validate() const override;
    /**
     * @brief Validates the relations of the data to other content pieces with regard to a given content holder
     * @param content the content holder to validate the relations against
     * @return the errors that occured during validation
     */
    virtual Errors validate_relations(const Content& content) const override;

    SpellcastingData spellcasting_data;
    std::vector<FeatureData> features_data;
    std::string subclass_feature_name;
    DiceData hit_dice_data;
    ImportantLevelsData important_levels_data;
};

} // namespace dnd

#endif // CHARACTER_CLASS_DATA_HPP_
