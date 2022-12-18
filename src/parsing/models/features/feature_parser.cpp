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
            parseAndAddEffect(effect_str, feature);
        } catch (const std::invalid_argument& e) {
            throw std::invalid_argument("Feature \"" + feature.name + "\": " + e.what());
        }
    }
}

void dnd::FeatureParser::parseAndAddEffect(const std::string& effect_str, Feature& feature) {
    const std::string times = "(earliest|early|normal|late|latest)";
    const std::string numeric_effects = "(add|mult|div|set)";
    const std::string numeric_effects_regex = "(" + numeric_effects + " -?\\d+(\\.\\d+)?)";
    const std::string identifier_effects = "(addOther|multOther|divOther|setOther|addConst|multConst|divConst|setConst)";
    const std::string identifier_effects_regex = "("+identifier_effects+" [A-Z0-9]+)";
    const std::regex effect_regex(
        "[A-Z0-9]+ " + times + " (" + numeric_effects_regex + "|" + identifier_effects_regex + ")"
    );
    if (!std::regex_match(effect_str, effect_regex)) {
        throw std::invalid_argument("Effect is of wrong format.");
    }
    std::string::const_iterator it = effect_str.cbegin();
    while (*it != ' ') {
        it++;
    }
    const std::string affected_attribute(effect_str.cbegin(), it);
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

    const std::string last_part = std::string(++it, effect_str.cend());
    std::unique_ptr<Effect> effect_ptr;
    if (effect_type.size() < 5) {
        const float effect_value = std::stof(last_part);
        if (effect_type == "add") {
            effect_ptr = std::make_unique<AddEffect>(affected_attribute, effect_value * 100);
            // attributes are stored as integers * 100, see CreatureState
        } else if (effect_type == "mult") {
            effect_ptr = std::make_unique<MultEffect>(affected_attribute, effect_value);
        } else if (effect_type == "div") {
            effect_ptr = std::make_unique<DivEffect>(affected_attribute, effect_value);
        } else if (effect_type == "set") {
            effect_ptr = std::make_unique<SetEffect>(affected_attribute, effect_value * 100);
            // attributes are stored as integers * 100, see CreatureState
        }
    } else {
        if (effect_type == "addOther") {
            effect_ptr = std::make_unique<AddOtherEffect>(affected_attribute, last_part);
        } else if (effect_type == "multOther") {
            effect_ptr = std::make_unique<MultOtherEffect>(affected_attribute, last_part);
        } else if (effect_type == "divOther") {
            effect_ptr = std::make_unique<DivOtherEffect>(affected_attribute, last_part);
        } else if (effect_type == "setOther") {
            effect_ptr = std::make_unique<SetOtherEffect>(affected_attribute, last_part);
        } else if (effect_type == "addConst") {
            effect_ptr = std::make_unique<AddConstEffect>(affected_attribute, last_part);
        } else if (effect_type == "multConst") {
            effect_ptr = std::make_unique<MultConstEffect>(affected_attribute, last_part);
        } else if (effect_type == "divConst") {
            effect_ptr = std::make_unique<DivConstEffect>(affected_attribute, last_part);
        } else if (effect_type == "setConst") {
            effect_ptr = std::make_unique<SetConstEffect>(affected_attribute, last_part);
        }
    }

    if (effect_time == "earliest") {
        feature.earliest.push_back(std::move(effect_ptr));
    } else if (effect_time == "early") {
        feature.early.push_back(std::move(effect_ptr));
    } else if (effect_time == "normal") {
        feature.normal.push_back(std::move(effect_ptr));
    } else if (effect_time == "late") {
        feature.late.push_back(std::move(effect_ptr));
    } else if (effect_time == "latest") {
        feature.latest.push_back(std::move(effect_ptr));
    }
}
