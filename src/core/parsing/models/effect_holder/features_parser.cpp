#include "dnd_config.hpp"

#include "features_parser.hpp"

#include <regex>
#include <string>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include "models/effect_holder/feature.hpp"
#include "parsing/content_file_parser.hpp"
#include "parsing/parse_optionals.hpp"
#include "parsing/parsing_exceptions.hpp"

void dnd::FeaturesParser::parseFeatures(const nlohmann::json& features_json) {
    DND_MEASURE_FUNCTION();

    if (!features_json.is_object()) {
        throw attribute_format_error(type, filepath, "features", "map/object");
    }

    features.reserve(features_json.size());

    for (const auto& [feature_name, feature_json] : features_json.items()) {
        features.emplace_back(createFeature(feature_name, feature_json));
    }
}

dnd::Feature dnd::FeaturesParser::createFeature(const std::string& feature_name, const nlohmann::json& feature_json)
    const {
    const std::string description = feature_json.at("description").get<std::string>();

    // TODO: change feature constructor?
    Feature feature(feature_name, description);

    feature.main_part = effect_holder_parser.createEffectHolder(feature_json);
    if (feature_json.contains("choose")) {
        feature.parts_with_choices.emplace_back(effect_holder_parser.createEffectHolderWithChoices(
            nlohmann::json::object({{"choose", feature_json.at("choose")}})
        ));
    }
    if (feature_json.contains("multi")) {
        if (!feature_json.at("multi").is_array()) {
            throw attribute_format_error(type, filepath, "multi", "array");
        }
        if (feature_json.at("multi").empty()) {
            throw invalid_attribute(type, filepath, "multi", "cannot be empty");
        }
        for (const auto& part_json : feature_json) {
            if (part_json.empty()) {
                throw invalid_attribute(type, filepath, "multi", "cannot have empty entry");
            }
            if (part_json.contains("choose")) {
                feature.parts_with_choices.emplace_back(effect_holder_parser.createEffectHolderWithChoices(part_json));
            } else {
                feature.parts.emplace_back(effect_holder_parser.createEffectHolder(part_json));
            }
        }
    }

    dnd::parseOptional(feature_json, "subclass", feature.subclass);

    return feature;
}
