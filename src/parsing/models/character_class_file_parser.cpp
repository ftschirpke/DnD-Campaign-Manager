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
    if (!json_to_parse.is_object()) {
        throw json_format_error(ParsingType::CLASSES, filename, "map/object");
    }
    character_class_name = json_to_parse.at("name").get<std::string>();
    if (character_class_name.size() == 0) {
        throw invalid_attribute(ParsingType::CLASSES, filename, "name", "cannot be \"\".");
    }
    character_class_hit_dice = json_to_parse.at("hit_dice").get<std::string>();
    // TODO: change int to short
    asi_levels = json_to_parse.at("asi_levels").get<std::vector<int>>();

    try {
        parseFeatures();
    } catch (parsing_error& e) {
        e.setParsingType(ParsingType::CLASSES);
        throw e;
    }

    const Feature* subclass_feature = nullptr;
    for (auto it = features.cbegin(); it != features.cend(); ++it) {
        if ((*it)->subclass) {
            if (subclass_feature != nullptr) {
                throw invalid_attribute(
                    ParsingType::CLASSES, filename, "features", "there must be only one subclass feature."
                );
            }
            subclass_feature = it->get();
        }
    }
    if (subclass_feature == nullptr) {
        throw invalid_attribute(ParsingType::CLASSES, filename, "features", "there must be one subclass feature.");
    }

    subclass_level = 1;
    while (!subclass_feature->isActiveForLevel(subclass_level)) {
        ++subclass_level;
    }
    if (subclass_level < 1 || subclass_level > 20) {
        throw invalid_attribute(
            ParsingType::CLASSES, filename, "features", "subclass feature must be active for a level between 1 and 20."
        );
    }
}

bool dnd::CharacterClassFileParser::validate() const {
    if (results.find(character_class_name) != results.end()) {
        std::cerr << "Warning: Duplicate of class \"" << character_class_name << "\" found.\n";
        return false;
    }
    return true;
}

void dnd::CharacterClassFileParser::saveResult() {
    // TODO: change CharacterClass constructor
    auto character_class =
        std::make_shared<CharacterClass>(character_class_name, character_class_hit_dice, asi_levels, subclass_level);
    character_class->features = std::move(features);
    results.emplace(character_class_name, std::move(character_class));
}

void dnd::CharacterClassFileParser::reset() {
    FeatureHolderFileParser::reset();
    character_class_name = "";
    character_class_hit_dice = "";
    subclass_level = 0;
    asi_levels = {};
}
