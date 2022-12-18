#include "feature_parser.hpp"

#include <memory>
#include <regex>
#include <stdexcept>

#include <nlohmann/json.hpp>

#include "../../../models/features/effect.hpp"
#include "../../../models/features/feature.hpp"

std::unique_ptr<dnd::Feature> dnd::FeatureParser::createFeature(
    const std::string& feature_name, const nlohmann::json& feature_json) {
    if (!feature_json.is_object()) {
        throw std::invalid_argument(
            "Feature \"" + feature_name + "\" is not formatted as an object/map."
        );
    }
    Feature feature(feature_name, feature_json.at("description"));
    if (feature_json.contains("effects")) {
        addEffects(feature_json.at("effects"), feature);
    }
    return std::make_unique<Feature>(std::move(feature));
}


void dnd::FeatureParser::addEffects(const nlohmann::json& effects_json, Feature& feature) {
    if (!effects_json.is_array()) {
        throw std::invalid_argument(
            "Effects of feature \"" + feature.name + "\" are not formatted as an array."
        );
    }
    for (const std::string& effect_str : effects_json) {
        try {
            parseEffect(effect_str, feature);
        } catch (const std::invalid_argument& e) {
            throw std::invalid_argument("Feature \"" + feature.name + "\": " + e.what());
        }
    }
}

void dnd::FeatureParser::parseEffect(const std::string& effect_str, Feature& feature) {
    const std::regex effect_regex(
        "[A-Z]+ (earliest|early|normal|late|latest) (add|mult|div|set) -?(\\d+(\\.\\d+)?)"
    );
    if (!std::regex_match(effect_str, effect_regex)) {
        throw std::invalid_argument("Effect is of wrong format.");
    }
    std::string::const_iterator it = effect_str.cbegin();
    while (*it != ' ') {
        it++;
    }
    const std::string affected_value_name(effect_str.cbegin(), it);
    it++;
    std::string::const_iterator start_it = it;
    while (*it != ' ') {
        it++;
    }
    const std::string effect_time(start_it, it);
    start_it = ++it;
    while (*it != ' ') {
        it++;
    }
    const std::string effect_type(start_it, it);
    const float effect_value = std::stof(std::string(++it, effect_str.cend()));

    std::unique_ptr<Effect> effect_ptr;
    if (effect_type == "add") {
        effect_ptr = std::make_unique<AddEffect>(effect_value * 100);
        // attributes are stored as integers * 100, see CreatureState
    } else if (effect_type == "mult") {
        effect_ptr = std::make_unique<MultEffect>(effect_value);
    } else if (effect_type == "div") {
        effect_ptr = std::make_unique<DivEffect>(effect_value);
    } else if (effect_type == "set") {
        effect_ptr = std::make_unique<SetEffect>(effect_value * 100);
        // attributes are stored as integers * 100, see CreatureState
    }

    if (effect_time == "earliest") {
        feature.earliest.emplace(affected_value_name, std::move(effect_ptr));
    } else if (effect_time == "early") {
        feature.early.emplace(affected_value_name, std::move(effect_ptr));
    } else if (effect_time == "normal") {
        feature.normal.emplace(affected_value_name, std::move(effect_ptr));
    } else if (effect_time == "late") {
        feature.late.emplace(affected_value_name, std::move(effect_ptr));
    } else if (effect_time == "latest") {
        feature.latest.emplace(affected_value_name, std::move(effect_ptr));
    }
}
