#ifndef FEATURE_DATA_HPP_
#define FEATURE_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <memory>

#include <core/errors/errors.hpp>
#include <core/validation/effects/effects_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class FeatureData : public ValidationData {
public:
    explicit FeatureData(const ValidationData* parent = nullptr) noexcept;
    std::strong_ordering operator<=>(const FeatureData&) const noexcept = default;
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

    const ValidationData* get_parent() const noexcept;

    EffectsData main_part_data;
    std::vector<EffectsData> other_parts_data;
private:
    const ValidationData* parent;
};

} // namespace dnd

#endif // FEATURE_DATA_HPP_
