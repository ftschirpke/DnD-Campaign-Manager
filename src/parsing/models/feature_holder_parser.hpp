#ifndef FEATURE_HOLDER_PARSER_HPP_
#define FEATURE_HOLDER_PARSER_HPP_

#include <nlohmann/json.hpp>

#include "../../models/feature_holder.hpp"

namespace dnd {

class FeatureHolderParser {
public:
    static void parseAndAddFeatures(const nlohmann::json& features_json,
        FeatureHolder& feature_holder);
};

} // namespace dnd

#endif // FEATURE_HOLDER_PARSER_HPP_
