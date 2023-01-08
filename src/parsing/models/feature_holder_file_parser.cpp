#include "dnd_config.hpp"

#include "feature_holder_file_parser.hpp"

#include <memory>
#include <regex>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "models/creature_state.hpp"
#include "models/effects_holder/feature.hpp"
#include "parsing/content_file_parser.hpp"
#include "parsing/parsing_exceptions.hpp"

void dnd::FeatureHolderFileParser::parseFeatures() {
    const nlohmann::json& features_json = json_to_parse.at("features");
    if (!features_json.is_object()) {
        throw attribute_format_error(filepath, "features", "map/object");
    }

    features.reserve(features_json.size());

    for (const auto& [feature_name, feature_json] : features_json.items()) {
        features.emplace_back(createFeature(feature_name, feature_json));
    }
}

dnd::Feature dnd::FeatureHolderFileParser::createFeature(
    const std::string& feature_name, const nlohmann::json& feature_json
) const {
    // TODO: change feature constructor?
    Feature feature(std::move(createEffectsHolder(feature_name, feature_json)));

    dnd::parseOptional(feature_json, "subclass", feature.subclass);

    return feature;
}
