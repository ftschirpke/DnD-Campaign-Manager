#include "dnd_config.hpp"

#include "feature_holder_file_parser.hpp"

#include <memory>
#include <regex>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "models/creature_state.hpp"
#include "models/features/activation.hpp"
#include "models/features/feature.hpp"
#include "parsing/parsing_exceptions.hpp"
#include "parsing/parsing_types.hpp"

void dnd::FeatureHolderFileParser::parseFeatures() {
    const nlohmann::json& features_json = json_to_parse.at("features");
    if (!features_json.is_object()) {
        throw attribute_format_error(filename, "features", "map/object");
    }

    features.reserve(features_json.size());

    for (const auto& [feature_name, feature_json] : features_json.items()) {
        features.emplace_back(createFeature(feature_name, feature_json));
    }
}

std::shared_ptr<dnd::Feature> dnd::FeatureHolderFileParser::createFeature(
    const std::string& feature_name, const nlohmann::json& feature_json
) const {
    if (!feature_json.is_object()) {
        throw attribute_format_error(filename, feature_name, "map/object");
    }
    const std::string feature_description = feature_json.at("description").get<std::string>();

    // TODO: change feature constructor?
    Feature feature(feature_name, feature_description);

    feature.subclass = false;
    if (feature_json.contains("subclass")) {
        feature.subclass = feature_json.at("subclass").get<bool>();
    }

    if (feature_json.contains("activation") && feature_json.contains("activations")) {
        throw invalid_attribute(
            filename, feature_name + ":activation/activations", "attributes are mutally exclusive."
        );
    } else if (feature_json.contains("activation")) {
        const std::string activation_str = feature_json.at("activation").get<std::string>();
        parseAndAddActivation(activation_str, feature);
    } else if (feature_json.contains("activations")) {
        if (!feature_json.at("activations").is_array()) {
            throw attribute_format_error(filename, feature_name + ":activations", "array");
        }
        const std::vector<std::string> activation_strs = feature_json.at("activations").get<std::vector<std::string>>();
        for (const std::string& activation_str : activation_strs) {
            parseAndAddActivation(activation_str, feature);
        }
    }

    if (feature_json.contains("effects")) {
        if (!feature_json.at("effects").is_array()) {
            throw attribute_format_error(filename, feature_name + ":effects", "array");
        }
        for (const auto& effect_val : feature_json.at("effects")) {
            const std::string effect_str = effect_val.get<std::string>();
            parseAndAddEffect(effect_str, feature);
        }
    }
    return std::make_shared<Feature>(std::move(feature));
}

void dnd::FeatureHolderFileParser::parseAndAddEffect(const std::string& effect_str, dnd::Feature& feature) const {
    const std::regex effect_regex("[A-Z][_A-Z0-9]+"
                                  " "
                                  "(earliest|early|normal|late|latest)"
                                  " "
                                  "("
                                  "("
                                  "(add|mult|div|set|max|min) -?\\d+(\\.\\d\\d?)?"
                                  ")"
                                  "|"
                                  "("
                                  "addOther|multOther|divOther|setOther|maxOther|minOther"
                                  "|"
                                  "addConst|multConst|divConst|setConst|maxConst|minConst"
                                  ")"
                                  " "
                                  "[A-Z][_A-Z0-9]+"
                                  ")");
    if (!std::regex_match(effect_str, effect_regex)) {
        throw attribute_type_error(
            filename, "feature \"" + feature.name + "\": invalid effect format: \"" + effect_str + "\""
        );
    }
    std::string::const_iterator it = effect_str.cbegin();
    while (*it != ' ') {
        ++it;
    }
    const std::string affected_attribute(effect_str.cbegin(), it);
    ++it;
    std::string::const_iterator start_it = it;
    while (*it != ' ') {
        ++it;
    }
    const std::string effect_time_str(start_it, it);
    start_it = ++it;
    while (*it != ' ') {
        ++it;
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

void dnd::FeatureHolderFileParser::parseAndAddActivation(const std::string& activation_str, Feature& feature) const {
    const std::string operators_allowed = "(==|!=|>=|<=|>|<)";
    const std::regex activation_regex(
        "[A-Z][_A-Z0-9]+ " + operators_allowed + " ([A-Z][_A-Z0-9]+|-?\\d+(\\.\\d\\d?)?|true|false)"
    );
    if (!std::regex_match(activation_str, activation_regex)) {
        throw attribute_type_error(
            filename, "feature \"" + feature.name + "\": invalid activation format: \"" + activation_str + "\""
        );
    }
    std::string::const_iterator it = activation_str.cbegin();
    while (*it != ' ') {
        ++it;
    }
    const std::string left_identifier(activation_str.cbegin(), it);
    ++it;
    const std::string::const_iterator last_it = it;
    while (*it != ' ') {
        ++it;
    }
    const std::string op_name(last_it, it);
    ++it;
    const std::string last_part(it, activation_str.cend());

    if (last_part[0] >= 'A' && last_part[0] <= 'Z') {
        feature.activations.emplace_back(std::make_unique<IdentifierActivation>(left_identifier, op_name, last_part));
        return;
    }

    int right_value;
    if (last_part == "true") {
        right_value = true;
    } else if (last_part == "false") {
        right_value = false;
    } else {
        right_value = std::stof(last_part) * 100;
        // attributes are stored as integers * 100, see CreatureState
    }
    feature.activations.emplace_back(std::make_unique<NumericActivation>(left_identifier, op_name, right_value));
}

void dnd::FeatureHolderFileParser::reset() { features = {}; }
