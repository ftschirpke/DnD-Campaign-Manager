#include <dnd_config.hpp>

#include "class_feature_parser.hpp"

#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/models/effects_provider/class_feature.hpp>
#include <core/parsing/effects/effects_parser.hpp>
#include <core/parsing/parser.hpp>

namespace dnd {

ClassFeatureParser::ClassFeatureParser(const std::filesystem::path& filepath)
    : Parser(filepath), effects_parser(filepath) {}

Errors ClassFeatureParser::parse_into(nlohmann::ordered_json&& json, ClassFeature::Data& data) const {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(
            ParsingError::Code::INVALID_FILE_FORMAT, get_filepath(), "The class feature json is not an object."
        );
        return errors;
    }

    errors += parse_required_attribute_into(json, "description", data.description);
    json.erase("description");
    data.source_path = get_filepath();

    errors += parse_optional_attribute_into(json, "level", data.level);

    errors += effects_parser.parse_into(std::move(json), data.main_effects_data);
    if (json.contains("higher_levels")) {
        if (json["higher_levels"].is_array()) {
            for (auto& effects : json["higher_levels"]) {
                int level;
                errors += parse_required_attribute_into(effects, "level", level);
                auto [inserted_pair_it, was_inserted] = data.higher_level_effects_data.emplace(level, Effects::Data());
                if (!was_inserted) {
                    errors.add_parsing_error(
                        ParsingError::Code::INVALID_FILE_FORMAT, get_filepath(),
                        "The class feature json's \"higher_levels\" contains multiple effects for the same level. "
                        "Consider merging them if that was intended."
                    );
                    continue;
                }
                errors += effects_parser.parse_into(std::move(effects), inserted_pair_it->second);
            }
        } else {
            errors.add_parsing_error(
                ParsingError::Code::INVALID_FILE_FORMAT, get_filepath(),
                "The class feature json's \"higher_levels\" is not an array."
            );
        }
        json.erase("higher_levels");
    }
    return errors;
}

Errors ClassFeatureParser::parse_multiple_into(nlohmann::ordered_json&& json, std::vector<ClassFeature::Data>& data)
    const {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(
            ParsingError::Code::INVALID_FILE_FORMAT, get_filepath(), "The class feature json is not an object."
        );
        return errors;
    }

    for (auto& [class_feature_name, class_feature_json] : json.items()) {
        ClassFeature::Data& class_feature_data = data.emplace_back();
        class_feature_data.name = class_feature_name;
        class_feature_data.source_path = get_filepath();
        errors += parse_into(std::move(class_feature_json), class_feature_data);
    }

    return errors;
}

} // namespace dnd
