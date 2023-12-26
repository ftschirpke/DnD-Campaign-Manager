#ifndef CHARACTER_RACE_DATA_HPP_
#define CHARACTER_RACE_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <memory>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/validation/effects_provider/feature_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class CharacterRaceData : public ValidationData {
public:
    std::strong_ordering operator<=>(const CharacterRaceData&) const noexcept = default;
    virtual std::unique_ptr<ValidationData> pack() const override;
    virtual Errors validate() const override;
    virtual Errors validate_relations(const Content& content) const override;

    std::vector<FeatureData> features_data;
    bool subraces;
};

} // namespace dnd

#endif // CHARACTER_RACE_DATA_HPP_
