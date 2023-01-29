#include "dnd_config.hpp"

#include "character_class_file_parser.hpp"

#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include "models/character_class.hpp"
#include "models/effect_holder/feature.hpp"
#include "parsing/models/effect_holder/features_parser.hpp"
#include "parsing/models/spellcasting/spellcasting_parser.hpp"
#include "parsing/parsing_exceptions.hpp"
#include "parsing/parsing_types.hpp"

void dnd::CharacterClassFileParser::parse() {
    DND_MEASURE_FUNCTION();
    if (!json_to_parse.is_object()) {
        throw json_format_error(type, filepath, "map/object");
    }
    character_class_name = json_to_parse.at("name").get<std::string>();
    if (character_class_name.empty()) {
        throw invalid_attribute(type, filepath, "name", "cannot be \"\".");
    }
    character_class_hit_dice = json_to_parse.at("hit_dice").get<std::string>();
    // TODO: change int to short
    asi_levels = json_to_parse.at("asi_levels").get<std::vector<int>>();

    features_parser.parseFeatures(json_to_parse.at("features"));

    determineSubclassLevel(features_parser.getFeatures());

    if (json_to_parse.contains("spellcasting")) {
        spellcasting_parser.parseSpellcasting(json_to_parse.at("spellcasting"));
    }
}

void dnd::CharacterClassFileParser::determineSubclassLevel(const std::vector<dnd::Feature>& features) {
    DND_MEASURE_FUNCTION();
    const Feature* subclass_feature = nullptr;
    for (const auto& feature : features) {
        if (feature.subclass) {
            if (subclass_feature != nullptr) {
                throw invalid_attribute(type, filepath, "features", "there must be only one subclass feature.");
            }
            subclass_feature = &feature;
        }
    }
    if (subclass_feature == nullptr) {
        throw invalid_attribute(type, filepath, "features", "there must be one subclass feature.");
    }

    subclass_level = 1;
    while (!subclass_feature->isActiveForLevel(subclass_level)) {
        ++subclass_level;
    }
    if (subclass_level < 1 || subclass_level > 20) {
        throw invalid_attribute(
            type, filepath, "features", "subclass feature must be active for a level between 1 and 20."
        );
    }
}

bool dnd::CharacterClassFileParser::validate() const {
    if (classes.contains(character_class_name)) {
        std::cerr << "Warning: Duplicate of class \"" << character_class_name << "\" found.\n";
        return false;
    }
    return true;
}

void dnd::CharacterClassFileParser::saveResult() {
    classes.emplace(
        std::piecewise_construct, std::forward_as_tuple(character_class_name),
        std::forward_as_tuple(
            character_class_name, std::move(features_parser.retrieveFeatures()), character_class_hit_dice, asi_levels,
            subclass_level
        )
    );
    // TODO: add spellcasting
}
