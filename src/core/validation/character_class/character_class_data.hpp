#ifndef CHARACTER_CLASS_DATA_HPP_
#define CHARACTER_CLASS_DATA_HPP_

#include <dnd_config.hpp>

#include <memory>
#include <string>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/validation/basic_mechanics/dice_data.hpp>
#include <core/validation/character_class/important_levels_data.hpp>
#include <core/validation/character_class/spellcasting/spellcasting_data.hpp>
#include <core/validation/feature/feature_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class ContentHolder;

class CharacterClassData : public ValidationData {
public:
    CharacterClassData() noexcept;
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
    virtual Errors validate_relations(const ContentHolder& content) const override;

    SpellcastingData spellcasting_data;
    std::vector<FeatureData> features_data;
    std::string subclass_feature_name;
    DiceData hit_dice_data;
    ImportantLevelsData important_levels_data;
};

} // namespace dnd

#endif // CHARACTER_CLASS_DATA_HPP_
