#include "feature_parser.hpp"

#include <memory>
#include <regex>
#include <stdexcept>
#include <string>

#include <nlohmann/json.hpp>

#include "models/creature_state.hpp"
#include "models/features/effect.hpp"
#include "models/features/feature.hpp"

std::shared_ptr<dnd::Feature> dnd::FeatureParser::createFeature(
    const std::string& feature_name, const nlohmann::json& feature_json
) {
    if (!feature_json.is_object()) {
        throw std::invalid_argument("Feature \"" + feature_name + "\" is not formatted as an object/map.");
    }
    const std::string feature_description = feature_json.at("description").get<std::string>();
    Feature feature(feature_name, feature_description);
    if (feature_json.contains("effects")) {
        addEffects(feature_json.at("effects"), feature);
    }
    return std::make_shared<Feature>(std::move(feature));
}


void dnd::FeatureParser::addEffects(const nlohmann::json& effects_json, Feature& feature) {
    if (!effects_json.is_array()) {
        throw std::invalid_argument("Effects of feature \"" + feature.name + "\" are not formatted as an array.");
    }
    for (const auto& effect_val : effects_json) {
        try {
            const std::string effect_str = effect_val.get<std::string>();
            parseAndAddEffect(effect_str, feature);
        } catch (const std::invalid_argument& e) {
            throw std::invalid_argument("Feature \"" + feature.name + "\": " + e.what());
        }
    }
}

void dnd::FeatureParser::parseAndAddEffect(const std::string& effect_str, Feature& feature) {
    const std::string times = "(earliest|early|normal|late|latest)";
    const std::string numeric_effects = "(add|mult|div|set|max|min)";
    const std::string numeric_effects_regex = "(" + numeric_effects + " -?\\d+(\\.\\d\\d?)?)";
    const std::string other_effects = "addOther|multOther|divOther|setOther|maxOther|minOther";
    const std::string const_effects = "addConst|multConst|divConst|setConst|maxConst|minConst";
    const std::string identifier_effects = "(" + other_effects + "|" + const_effects + ")";
    const std::string identifier_effects_regex = "(" + identifier_effects + " [A-Z][_A-Z0-9]+)";
    const std::regex effect_regex(
        "[A-Z][_A-Z0-9]+ " + times + " (" + numeric_effects_regex + "|" + identifier_effects_regex + ")"
    );
    if (!std::regex_match(effect_str, effect_regex)) {
        throw std::invalid_argument("Effect \"" + effect_str + "\" is of wrong format.");
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
    const std::string effect_time_str(start_it, it);
    start_it = ++it;
    while (*it != ' ') {
        it++;
    }
    const std::string effect_type(start_it, it);

    const std::string last_part = std::string(++it, effect_str.cend());
    std::unique_ptr<Effect> effect_ptr;
    if (effect_type.size() < 5) {
        const float effect_value = std::stof(last_part);
        if (effect_type == "mult" || effect_type == "div") {
            effect_ptr = std::make_unique<FloatNumEffect>(affected_attribute, effect_type, effect_value);
        } else {
            effect_ptr = std::make_unique<IntNumEffect>(affected_attribute, effect_type, effect_value * 100);
            // attributes are stored as integers * 100, see CreatureState
        }
    } else {
        int other_idx = effect_type.find("Other");
        int const_idx = effect_type.find("Const");
        if (other_idx != std::string::npos) {
            const std::string op_name(effect_type.cbegin(), effect_type.cbegin() + other_idx);
            effect_ptr = std::make_unique<OtherAttributeEffect>(affected_attribute, op_name, last_part);
        } else if (const_idx != std::string::npos) {
            const std::string op_name(effect_type.cbegin(), effect_type.cbegin() + const_idx);
            effect_ptr = std::make_unique<ConstEffect>(affected_attribute, op_name, last_part);
        }
    }

    const EffectTime effect_time = effect_time_for_string.at(effect_time_str);
    if (CreatureState::isAbility(affected_attribute)) {
        feature.ability_score_effects[effect_time].push_back(std::move(effect_ptr));
    } else {
        feature.normal_effects[effect_time].push_back(std::move(effect_ptr));
    }
}
