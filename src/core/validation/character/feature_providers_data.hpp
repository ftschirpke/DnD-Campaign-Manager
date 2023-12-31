#ifndef FEATURE_PROVIDERS_DATA_HPP_
#define FEATURE_PROVIDERS_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <string>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class Content;

class FeatureProvidersData : public ValidationSubdata {
public:
    FeatureProvidersData(const ValidationData* parent) noexcept;
    std::strong_ordering operator<=>(const FeatureProvidersData&) const noexcept = default;

    std::string species_name;
    std::string subspecies_name;
    std::string class_name;
    std::string subclass_name;
};

} // namespace dnd

#endif // FEATURE_PROVIDERS_DATA_HPP_
