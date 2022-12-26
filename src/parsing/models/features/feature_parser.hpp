#ifndef FEATURE_PARSER_HPP_
#define FEATURE_PARSER_HPP_

#include <memory>

#include <nlohmann/json.hpp>

#include "models/features/feature.hpp"

namespace dnd {

class FeatureParser {
public:
    static void addEffects(const nlohmann::json& effects_json, Feature& feature);
    static void parseAndAddEffect(const std::string& effect_str, Feature& feature);
    static std::shared_ptr<Feature> createFeature(const std::string& feature_name, const nlohmann::json& feature_json);
};

} // namespace dnd

#endif // FEATURE_PARSER_HPP_
