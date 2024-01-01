#ifndef CHOOSABLE_DATA_HPP_
#define CHOOSABLE_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <memory>
#include <vector>

#include <core/validation/effects/condition/condition_data.hpp>
#include <core/validation/effects_provider/feature_data.hpp>

namespace dnd {

class ChoosableData : public FeatureData {
public:
    std::strong_ordering operator<=>(const ChoosableData&) const noexcept = default;

    std::string type;
    std::vector<ConditionData> prerequisites_data;
};

Errors validate_choosable_nonrecursively(const ChoosableData& data);
Errors validate_choosable_recursively_for_content(const ChoosableData& data, const Content& content);

} // namespace dnd

#endif // CHOOSABLE_DATA_HPP_
