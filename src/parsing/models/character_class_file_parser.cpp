#include "dnd_config.hpp"

#include "character_class_file_parser.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "models/character_class.hpp"
#include "parsing/models/feature_holder_file_parser.hpp"
#include "parsing/parsing_exceptions.hpp"
#include "parsing/parsing_types.hpp"

void dnd::CharacterClassFileParser::parse() {
    DND_MEASURE_FUNCTION();
    if (!json_to_parse.is_object()) {
        throw json_format_error(ParsingType::CLASS, filepath, "map/object");
    }
    character_class_name = json_to_parse.at("name").get<std::string>();
    if (character_class_name.size() == 0) {
        throw invalid_attribute(ParsingType::CLASS, filepath, "name", "cannot be \"\".");
    }
    character_class_hit_dice = json_to_parse.at("hit_dice").get<std::string>();
    // TODO: change int to short
    asi_levels = json_to_parse.at("asi_levels").get<std::vector<int>>();

    try {
        parseFeatures();
    } catch (parsing_error& e) {
        e.setParsingType(ParsingType::CLASS);
        throw e;
    }
    subclass_level = determineSubclassLevel(getFeatures());

    if (json_to_parse.contains("spellcasting")) {
        try {
            parseSpellcasting();
        } catch (parsing_error& e) {
            e.setParsingType(ParsingType::CLASS);
            throw e;
        }
    }
}

int dnd::CharacterClassFileParser::determineSubclassLevel(const std::vector<dnd::Feature>& features) const {
    DND_MEASURE_FUNCTION();
    const Feature* subclass_feature = nullptr;
    for (const auto& feature : features) {
        if (feature.subclass) {
            if (subclass_feature != nullptr) {
                throw invalid_attribute(
                    ParsingType::CLASS, filepath, "features", "there must be only one subclass feature."
                );
            }
            subclass_feature = &feature;
        }
    }
    if (subclass_feature == nullptr) {
        throw invalid_attribute(ParsingType::CLASS, filepath, "features", "there must be one subclass feature.");
    }

    int subclass_level = 1;
    while (!subclass_feature->isActiveForLevel(subclass_level)) {
        ++subclass_level;
    }
    if (subclass_level < 1 || subclass_level > 20) {
        throw invalid_attribute(
            ParsingType::CLASS, filepath, "features", "subclass feature must be active for a level between 1 and 20."
        );
    }
    return subclass_level;
}

bool dnd::CharacterClassFileParser::validate() const {
    if (results.find(character_class_name) != results.end()) {
        std::cerr << "Warning: Duplicate of class \"" << character_class_name << "\" found.\n";
        return false;
    }
    return true;
}

void dnd::CharacterClassFileParser::saveResult() {
    results.emplace(
        std::piecewise_construct, std::forward_as_tuple(character_class_name),
        std::forward_as_tuple(
            character_class_name, retrieveFeatures(), character_class_hit_dice, asi_levels, subclass_level
        )
    );
}
