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

    std::vector<FeatureData> features_data;
    std::string species_name;
};

Errors validate_subspecies_nonrecursively_for_content(const SubspeciesData& data, const Content& content);
Errors validate_subspecies_recursively_for_content(const SubspeciesData& data, const Content& content);

} // namespace dnd

#endif // SUBSPECIES_DATA_HPP_
