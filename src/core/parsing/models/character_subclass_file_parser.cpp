#include "dnd_config.hpp"

#include "character_subclass_file_parser.hpp"

#include <filesystem>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>

#include <nlohmann/json.hpp>

#include "models/character_class.hpp"
#include "models/character_subclass.hpp"
#include "parsing/models/effect_holder/features_parser.hpp"
#include "parsing/models/spellcasting/spellcasting_parser.hpp"
#include "parsing/parsing_exceptions.hpp"
#include "parsing/parsing_types.hpp"

void dnd::CharacterSubclassFileParser::parse() {
    DND_MEASURE_FUNCTION();
    if (!json_to_parse.is_object()) {
        throw json_format_error(type, filepath, "map/object");
    }
    character_subclass_name = json_to_parse.at("name").get<std::string>();
    if (character_subclass_name.empty()) {
        throw invalid_attribute(type, filepath, "name", "cannot be \"\".");
    }
    class_name = json_to_parse.at("class").get<std::string>();

    features_parser.parseFeatures(json_to_parse.at("features"));

    if (json_to_parse.contains("spellcasting")) {
        spellcasting_parser.parseSpellcasting(json_to_parse.at("spellcasting"));
    }
}

bool dnd::CharacterSubclassFileParser::validate() const {
    if (subclasses.contains(character_subclass_name)) {
        std::cerr << "Warning: Duplicate of subclass \"" << character_subclass_name << "\" found.\n";
        return false;
    }
    if (!classes.contains(class_name)) {
        throw invalid_attribute(type, filepath, "class", "must exist. \"" + class_name + "\" does not exist.");
    }
    return true;
}

void dnd::CharacterSubclassFileParser::saveResult() {
    subclasses.create(character_subclass_name, filepath, std::move(features_parser.retrieveFeatures()), class_name);
    // TODO: add spellcasting
}
