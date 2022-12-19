#include "feature_holder_parser.hpp"

#include <nlohmann/json.hpp>

#include "models/feature_holder.hpp"
#include "models/features/feature.hpp"
#include "parsing/models/features/feature_parser.hpp"

void dnd::FeatureHolderParser::parseAndAddFeatures(
    const nlohmann::json& features_json, FeatureHolder& feature_holder) {
    if (!features_json.is_object()) {
        throw std::invalid_argument(
            "Features for \"" + feature_holder.name + "\" are not formatted as an object/map."
        );
    }
    for (const auto& [feature_name, feature_info] : features_json.items()) {
        std::unique_ptr<Feature> feature = FeatureParser::createFeature(
            feature_name, feature_info
        );
        feature_holder.features.push_back(std::move(feature));
    }
}
