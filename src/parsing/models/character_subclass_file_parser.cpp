#include "dnd_config.hpp"

#include "character_subclass_file_parser.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "models/character_class.hpp"
#include "parsing/models/feature_holder_file_parser.hpp"
#include "parsing/parsing_exceptions.hpp"
#include "parsing/parsing_types.hpp"

void dnd::CharacterSubclassFileParser::parse() {
    DND_MEASURE_FUNCTION();
    if (!json_to_parse.is_object()) {
        throw json_format_error(ParsingType::SUBCLASS, filename, "map/object");
    }
    character_subclass_name = json_to_parse.at("name").get<std::string>();
    if (character_subclass_name.size() == 0) {
        throw invalid_attribute(ParsingType::SUBCLASS, filename, "name", "cannot be \"\".");
    }
    class_name = json_to_parse.at("class").get<std::string>();

    try {
        parseFeatures();
    } catch (parsing_error& e) {
        e.setParsingType(ParsingType::SUBCLASS);
        throw e;
    }
}

bool dnd::CharacterSubclassFileParser::validate() const {
    if (results.find(character_subclass_name) != results.end()) {
        std::cerr << "Warning: Duplicate of subclass \"" << character_subclass_name << "\" found.\n";
        return false;
    }
    if (classes.find(class_name) == classes.cend()) {
        throw invalid_attribute(
            ParsingType::SUBCLASS, filename, "class", "must exist. \"" + class_name + "\" does not exist."
        );
    }
    return true;
}

void dnd::CharacterSubclassFileParser::saveResult() {
    // TODO: change CharacterSubclass constructor
    auto character_subclass = std::make_shared<CharacterSubclass>(character_subclass_name, class_name);
    character_subclass->features = std::move(features);
    results.emplace(character_subclass_name, std::move(character_subclass));
}
