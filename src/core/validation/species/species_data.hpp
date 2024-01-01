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

    std::vector<FeatureData> features_data;
    bool subspecies;
};

Errors validate_species_nonrecursively_for_content(const SpeciesData& data, const Content& content);
Errors validate_species_recursively_for_content(const SpeciesData& data, const Content& content);

} // namespace dnd

#endif // SPECIES_DATA_HPP_
