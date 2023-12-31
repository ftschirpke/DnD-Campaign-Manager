#ifndef SUBSPECIES_DATA_HPP_
#define SUBSPECIES_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <memory>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/validation/effects_provider/feature_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class SubspeciesData : public ValidationData {
public:
    std::strong_ordering operator<=>(const SubspeciesData&) const noexcept = default;
    virtual std::unique_ptr<ValidationData> pack() const override;

    std::vector<FeatureData> features_data;
    std::string species_name;
};

} // namespace dnd

#endif // SUBSPECIES_DATA_HPP_
