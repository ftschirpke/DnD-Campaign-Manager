#include <dnd_config.hpp>

#include "choosable_parser.hpp"

#include <string>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/models/effects/condition/condition.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/parsing/effects_provider/feature_parser.hpp>
#include <core/parsing/parser.hpp>

namespace dnd {

ChoosableParser::ChoosableParser(const std::filesystem::path& filepath) : Parser(filepath), feature_parser(filepath) {}

Errors ChoosableParser::parse_into(nlohmann::ordered_json&& json, Choosable::Data& data) const {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(
            ParsingError::Code::INVALID_FILE_FORMAT, get_filepath(), "The choosable feature json is not an object."
        );
        return errors;
    }

    bool has_prerequisite = json.contains("prerequisite");
    bool has_prerequisites = json.contains("prerequisites");
    if (has_prerequisite && has_prerequisites) {
        errors.add_parsing_error(
            ParsingError::Code::UNEXPECTED_ATTRIBUTE, get_filepath(),
            "The effect holder json contains both \"prerequisite\" and \"prerequisites\"."
        );
    } else if (has_prerequisite) {
        Condition::Data& condition_data = data.prerequisites_data.emplace_back();
        errors += parse_optional_attribute_into(json, "prerequisite", condition_data.condition_str);
        json.erase("prerequisite");
    } else if (has_prerequisites) {
        std::vector<std::string> prerequisite_conditions_strs;
        errors += parse_optional_attribute_into(json, "prerequisites", prerequisite_conditions_strs);
        for (const std::string& prerequisite_condition_str : prerequisite_conditions_strs) {
            Condition::Data& condition_data = data.prerequisites_data.emplace_back();
            condition_data.condition_str = prerequisite_condition_str;
        }
        json.erase("prerequisites");
    }

    data.type = get_filepath().stem().string();

    errors += feature_parser.parse_into(std::move(json), data);

    return errors;
}

} // namespace dnd
