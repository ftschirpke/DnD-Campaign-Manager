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

    if (json.contains("higher_levels")) {
        if (json["higher_levels"].is_object()) {
            auto not_a_digit = [](unsigned char c) { return !std::isdigit(c); };
            for (auto& [level_str, effects] : json["higher_levels"].items()) {
                if (std::any_of(level_str.begin(), level_str.end(), not_a_digit)) {
                    errors.add_parsing_error(
                        ParsingErrorCode::INVALID_ATTRIBUTE_TYPE, get_filepath(),
                        "The feature json's \"higher_levels\"'s key, which should be the level at which the effect "
                        "activates, is not a number."
                    );
                    continue;
                }
                int level = std::stoi(level_str);
                auto [inserted_pair_it, was_inserted] = data.higher_level_effects_data.emplace(
                    level, EffectsData(data.get_parent())
                );
                if (!was_inserted) {
                    errors.add_parsing_error(
                        ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(),
                        "The feature json's \"higher_levels\"'s key, which should be the level at which the effect "
                        "activates, is not unique."
                    );
                    continue;
                }
                errors += effects_parser.parse(std::move(effects), inserted_pair_it->second);
            }
        } else {
            errors.add_parsing_error(
                ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(),
                "The feature json's \"higher_levels\" is not an object."
            );
        }
        json.erase("higher_levels");
    }
    errors += effects_parser.parse(std::move(json), data.main_effects_data);
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
