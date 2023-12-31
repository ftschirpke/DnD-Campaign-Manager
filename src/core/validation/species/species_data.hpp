#ifndef SPECIES_DATA_HPP_
#define SPECIES_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <memory>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/validation/effects_provider/feature_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class SpeciesData : public ValidationData {
public:
    std::strong_ordering operator<=>(const SpeciesData&) const noexcept = default;
    virtual std::unique_ptr<ValidationData> pack() const override;

    std::vector<FeatureData> features_data;
    bool subspecies;
};

} // namespace dnd

#endif // SPECIES_DATA_HPP_
