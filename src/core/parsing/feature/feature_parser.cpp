#include <dnd_config.hpp>

#include "feature_parser.hpp"

#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/parsing/effect_holder/effect_holder_parser.hpp>
#include <core/parsing/parser.hpp>
#include <core/validation/effect_holder/effect_holder_data.hpp>
#include <core/validation/feature/feature_data.hpp>
#include <core/validation/validation_data.hpp>

dnd::FeatureParser::FeatureParser(const std::filesystem::path& filepath) noexcept
    : Parser(filepath), effect_holder_parser(filepath) {}

dnd::Errors dnd::FeatureParser::parse(nlohmann::ordered_json&& json, FeatureData& data) const {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(
            ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(), "The feature json is not an object."
        );
        return errors;
    }

    if (json.contains("multi")) {
        if (json["multi"].is_array()) {
            for (auto& part : json["multi"]) {
                EffectHolderData& effect_holder_data = data.other_parts_data.emplace_back(data.get_parent());
                errors += effect_holder_parser.parse(std::move(part), effect_holder_data);
            }
        } else {
            errors.add_parsing_error(
                ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(), "The feature json's \"multi\" is not an array."
            );
        }
        json.erase("multi");
    }
    errors += effect_holder_parser.parse(std::move(json), data.main_part_data);
    return errors;
}
