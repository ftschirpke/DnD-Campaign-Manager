#ifndef CHARACTER_SUBRACE_DATA_HPP_
#define CHARACTER_SUBRACE_DATA_HPP_

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
    virtual Errors validate() const override;
    virtual Errors validate_relations(const Content& content) const override;

    std::vector<FeatureData> features_data;
    std::string species_name;
};

} // namespace dnd

#endif // CHARACTER_SUBRACE_DATA_HPP_
