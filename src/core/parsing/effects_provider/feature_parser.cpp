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
#include <core/validation/effects_provider/feature_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

FeatureParser::FeatureParser(const std::filesystem::path& filepath) noexcept
    : Parser(filepath), effects_parser(filepath) {}

Errors FeatureParser::parse_into(nlohmann::ordered_json&& json, FeatureData& data) const {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(
            ParsingError::Code::INVALID_FILE_FORMAT, get_filepath(), "The feature json is not an object."
        );
        return errors;
    }

    errors += parse_required_attribute_into(json, "description", data.description);
    json.erase("description");
    data.source_path = get_filepath();

    errors += effects_parser.parse_into(std::move(json), data.main_effects_data);
    return errors;
}

Errors FeatureParser::parse_multiple_into(
    nlohmann::ordered_json&& json, std::vector<FeatureData>& data, const ValidationData* parent
) const {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(
            ParsingError::Code::INVALID_FILE_FORMAT, get_filepath(), "The feature json is not an object."
        );
        return errors;
    }

    for (auto& [feature_name, feature_json] : json.items()) {
        FeatureData& feature_data = data.emplace_back(parent);
        feature_data.name = feature_name;
        feature_data.source_path = get_filepath();
        errors += parse_into(std::move(feature_json), feature_data);
    }

    return errors;
}

} // namespace dnd
