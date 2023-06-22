#ifndef FEATURE_DATA_HPP_
#define FEATURE_DATA_HPP_

#include <dnd_config.hpp>

#include <core/errors/errors.hpp>
#include <core/validation/effect_holder/effect_holder_data.hpp>
#include <core/validation/validation_data.hpp>
#include <memory>

namespace dnd {

class FeatureData : public ValidationData {
public:
    explicit FeatureData(const ValidationData* parent = nullptr) noexcept;
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

    const ValidationData* get_parent() const noexcept;

    EffectHolderData main_part_data;
    std::vector<EffectHolderData> other_parts_data;
private:
    const ValidationData* parent;
};

} // namespace dnd

#endif // FEATURE_DATA_HPP_
