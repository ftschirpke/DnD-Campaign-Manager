#include <dnd_config.hpp>

#include "features_parser.hpp"

#include <regex>
#include <string>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include <core/models/effect_holder/feature.hpp>
#include <core/parsing/content_file_parser.hpp>
#include <core/parsing/parse_optionals.hpp>
#include <core/parsing/parsing_exceptions.hpp>

void dnd::FeaturesParser::parse_features(const nlohmann::ordered_json& features_json) {
    DND_MEASURE_FUNCTION();

    if (!features_json.is_object()) {
        throw attribute_format_error(type, filepath, "features", "map/object");
    }

    features.reserve(features_json.size());

    for (const auto& [feature_name, feature_json] : features_json.items()) {
        features.emplace_back(create_feature(feature_name, feature_json));
    }
}

dnd::Feature dnd::FeaturesParser::create_feature(const std::string& feature_name, const nlohmann::json& feature_json)
    const {
    const std::string description = feature_json.at("description").get<std::string>();

    // TODO: change feature constructor?
    Feature feature(feature_name, filepath, description);

    feature.main_part = effect_holder_parser.create_effect_holder(feature_json);
    if (feature_json.contains("choose")) {
        feature.parts_with_choices.emplace_back(effect_holder_parser.create_effect_holder_with_choices(
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
                feature.parts_with_choices.emplace_back(effect_holder_parser.create_effect_holder_with_choices(part_json
                ));
            } else {
                feature.parts.emplace_back(effect_holder_parser.create_effect_holder(part_json));
            }
        }
    }

    dnd::parse_optional(feature_json, "subclass", feature.subclass);

    return feature;
}
