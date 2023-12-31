#ifndef CLASS_DATA_HPP_
#define CLASS_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <memory>
#include <string>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/validation/class/important_levels_data.hpp>
#include <core/validation/effects_provider/class_feature_data.hpp>
#include <core/validation/spellcasting/spellcasting_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class Content;

class ClassData : public ValidationData {
public:
    ClassData() noexcept;
    std::strong_ordering operator<=>(const ClassData&) const noexcept = default;
    virtual std::unique_ptr<ValidationData> pack() const override;

    SpellcastingData spellcasting_data;
    std::vector<ClassFeatureData> features_data;
    std::string subclass_feature_name;
    std::string hit_dice_str;
    ImportantLevelsData important_levels_data;
};

} // namespace dnd

#endif // CLASS_DATA_HPP_
