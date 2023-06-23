#ifndef CHARACTER_SUBRACE_DATA_HPP_
#define CHARACTER_SUBRACE_DATA_HPP_

#include <dnd_config.hpp>

#include <memory>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/validation/feature/feature_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class CharacterSubraceData : public ValidationData {
public:
    int operator<=>(const CharacterSubraceData&) const noexcept = default;
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

    std::vector<FeatureData> features_data;
    std::string race_name;
};

} // namespace dnd

#endif // CHARACTER_SUBRACE_DATA_HPP_
