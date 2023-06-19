#include <dnd_config.hpp>

#include "choosable_group_parser.hpp"

#include <filesystem>
#include <unordered_map>

#include <core/controllers/content_holder.hpp>
#include <core/errors/errors.hpp>
#include <core/models/feature/choosable_feature.hpp>
#include <core/parsing/feature/choosable_feature_parser.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/utils/char_manipulation.hpp>
#include <core/validation/feature/choosable_feature_data.hpp>

dnd::ChoosableGroupParser::ChoosableGroupParser(const std::filesystem::path& filepath) noexcept
    : FileParser(filepath), choosable_feature_parser(filepath) {}

dnd::Errors dnd::ChoosableGroupParser::parse() {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(
            ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(), "The choosable group json file is not an object."
        );
        return errors;
    }

    group_name = get_filepath().stem().string();

    bool first_in_word = true;
    for (auto& c : group_name) {
        if (c == '_') {
            c = ' ';
            first_in_word = true;
        }
        if (first_in_word) {
            c = char_to_uppercase(c);
            first_in_word = false;
        }
    }

    data.reserve(json.size());
    for (auto& [feature_name, feature_json] : json.items()) {
        if (!feature_json.is_object()) {
            errors.add_parsing_error(
                ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(),
                fmt::format("The feature '{}' is not an object.", feature_name)
            );
            continue;
        }
        Errors feature_errors;
        ChoosableFeatureData feature_data;
        feature_data.name = feature_name;
        feature_errors += choosable_feature_parser.parse(std::move(feature_json), feature_data);
        if (feature_errors.ok()) {
            data.emplace_back(std::move(feature_data));
        }
        errors += std::move(feature_errors);
    }
    return errors;
}

dnd::Errors dnd::ChoosableGroupParser::validate(const dnd::ContentHolder& content) const {
    Errors errors;
    feature_data_valid.resize(data.size(), false);
    for (size_t i = 0; i < data.size(); ++i) {
        Errors validation_errors = data[i].validate();
        validation_errors += data[i].validate_relations(content);
        feature_data_valid[i] = validation_errors.ok();
        errors += std::move(validation_errors);
    }
    return errors;
}

void dnd::ChoosableGroupParser::save_result(dnd::ContentHolder& content) {
    for (size_t i = 0; i < data.size(); ++i) {
        if (feature_data_valid[i]) {
            content.groups.add(group_name, ChoosableFeature::create(std::move(data[i]), content));
        }
    }
}
