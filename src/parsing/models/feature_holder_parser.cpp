#include "feature_holder_parser.hpp"

#include <memory>

#include <nlohmann/json.hpp>

#include "models/feature_holder.hpp"
#include "models/features/feature.hpp"
#include "parsing/models/features/feature_parser.hpp"

std::vector<std::shared_ptr<const dnd::Feature>> dnd::FeatureHolderParser::parseAndAddFeatures(
    const nlohmann::json& features_json
) {
    if (!features_json.is_object()) {
        throw std::invalid_argument("Features for \"" + "PLACEHOLDER" + "\" are not formatted as an object/map.");
    }
    std::vector<std::shared_ptr<const Feature>> features;
    for (const auto& [feature_name, feature_info] : features_json.items()) {
        std::shared_ptr<Feature> feature = FeatureParser::createFeature(feature_name, feature_info);
        features.push_back(std::move(feature));
    }
    return features;
}
