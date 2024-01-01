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

Errors validate_subclass_nonrecursively_for_content(const SubclassData& data, const Content& content);
Errors validate_subclass_recursively_for_content(const SubclassData& data, const Content& content);

} // namespace dnd

#endif // SUBCLASS_DATA_HPP_
