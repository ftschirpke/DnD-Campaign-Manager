#ifndef SUBCLASS_DATA_HPP_
#define SUBCLASS_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <memory>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/validation/effects_provider/class_feature_data.hpp>
#include <core/validation/spellcasting/spellcasting_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class SubclassData : public ValidationData {
public:
    SubclassData() noexcept;
    std::strong_ordering operator<=>(const SubclassData&) const noexcept = default;
    virtual std::unique_ptr<ValidationData> pack() const override;

    SpellcastingData spellcasting_data;
    std::vector<ClassFeatureData> features_data;
    std::string class_name;
};

} // namespace dnd

#endif // SUBCLASS_DATA_HPP_
