#ifndef CLASS_FEATURE_DATA_HPP_
#define CLASS_FEATURE_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <map>
#include <memory>

#include <core/errors/errors.hpp>
#include <core/validation/effects/effects_data.hpp>
#include <core/validation/effects_provider/feature_data.hpp>

namespace dnd {

class ClassFeatureData : public FeatureData {
public:
    explicit ClassFeatureData(const ValidationData* parent = nullptr) noexcept;
    std::strong_ordering operator<=>(const ClassFeatureData&) const noexcept = default;
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

    int level;
    std::map<int, EffectsData> higher_level_effects_data;
};

} // namespace dnd

#endif // CLASS_FEATURE_DATA_HPP_
