#ifndef ABILITY_SCORES_DATA_HPP_
#define ABILITY_SCORES_DATA_HPP_

#include <dnd_config.hpp>

#include <array>
#include <compare>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class AbilityScoresData : public ValidationSubdata {
public:
    AbilityScoresData(const ValidationData* parent) noexcept;
    std::strong_ordering operator<=>(const AbilityScoresData&) const noexcept = default;
    /**
     * @brief Validates the data
     * @return the errors that occured during validation
     */
    virtual Errors validate() const override;

    std::array<int, 6> ability_scores;
};

} // namespace dnd

#endif // ABILITY_SCORES_DATA_HPP_
