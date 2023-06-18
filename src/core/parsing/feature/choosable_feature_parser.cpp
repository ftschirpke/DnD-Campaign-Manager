#include <dnd_config.hpp>

#include "choosable_feature_parser.hpp"

#include <string>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/parsing/feature/feature_parser.hpp>
#include <core/parsing/parser.hpp>
#include <core/validation/effect_holder/condition/condition_data.hpp>
#include <core/validation/feature/choosable_feature_data.hpp>

dnd::ChoosableFeatureParser::ChoosableFeatureParser(const std::filesystem::path& filepath) noexcept
    : Parser(filepath), feature_parser(filepath) {}

dnd::Errors dnd::ChoosableFeatureParser::parse(nlohmann::ordered_json&& json, ChoosableFeatureData& data) const {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(
            ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(), "The choosable feature json is not an object."
        );
        return errors;
    }

    bool has_prerequisite = json.contains("prerequisite");
    bool has_prerequisites = json.contains("prerequisites");
    if (has_prerequisite && has_prerequisites) {
        errors.add_parsing_error(
            ParsingErrorCode::UNEXPECTED_ATTRIBUTE, get_filepath(),
            "The effect holder json contains both \"prerequisite\" and \"prerequisites\"."
        );
    } else if (has_prerequisite) {
        ConditionData& condition_data = data.prerequisites_data.emplace_back(&data);
        errors += parse_optional_attribute(json, "prerequisite", condition_data.condition_str);
        json.erase("prerequisite");
    } else if (has_prerequisites) {
        std::vector<std::string> prerequisite_conditions_strs;
        errors += parse_optional_attribute(json, "prerequisites", prerequisite_conditions_strs);
        for (const auto& prerequisite_condition_str : prerequisite_conditions_strs) {
            ConditionData& condition_data = data.prerequisites_data.emplace_back(&data);
            condition_data.condition_str = prerequisite_condition_str;
        }
        json.erase("prerequisites");
    }

    errors += feature_parser.parse(std::move(json), data);

    return errors;
}
