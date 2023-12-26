#ifndef CHARACTER_SUBCLASS_DATA_HPP_
#define CHARACTER_SUBCLASS_DATA_HPP_

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
    virtual Errors validate() const override;
    virtual Errors validate_relations(const Content& content) const override;

    SpellcastingData spellcasting_data;
    std::vector<ClassFeatureData> features_data;
    std::string class_name;
};

} // namespace dnd

#endif // CHARACTER_SUBCLASS_DATA_HPP_
