#include <dnd_config.hpp>

#include "feature_parser.hpp"

#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/parsing/effects/effects_parser.hpp>
#include <core/parsing/parser.hpp>
#include <core/validation/effects/effects_data.hpp>
#include <core/validation/feature/feature_data.hpp>
#include <core/validation/validation_data.hpp>

dnd::FeatureParser::FeatureParser(const std::filesystem::path& filepath) noexcept
    : Parser(filepath), effects_parser(filepath) {}

dnd::Errors dnd::FeatureParser::parse(nlohmann::ordered_json&& json, FeatureData& data) const {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(
            ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(), "The feature json is not an object."
        );
        return errors;
    }

    errors += parse_required_attribute(json, "description", data.description);
    json.erase("description");
    data.source_path = get_filepath();

    if (json.contains("multi")) {
        if (json["multi"].is_array()) {
            for (auto& part : json["multi"]) {
                EffectsData& effects_data = data.other_parts_data.emplace_back(data.get_parent());
                errors += effects_parser.parse(std::move(part), effects_data);
            }
        } else {
            errors.add_parsing_error(
                ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(), "The feature json's \"multi\" is not an array."
            );
        }
        json.erase("multi");
    }
    errors += effects_parser.parse(std::move(json), data.main_part_data);
    return errors;
}

dnd::Errors dnd::FeatureParser::parse_multiple(
    nlohmann::ordered_json&& json, std::vector<FeatureData>& data, const dnd::ValidationData* parent
) const {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(
            ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(), "The feature json is not an object."
        );
        return errors;
    }

    for (auto& [feature_name, feature_json] : json.items()) {
        FeatureData& feature_data = data.emplace_back(parent);
        feature_data.name = feature_name;
        feature_data.source_path = get_filepath();
        errors += parse(std::move(feature_json), feature_data);
    }

    return errors;
}
