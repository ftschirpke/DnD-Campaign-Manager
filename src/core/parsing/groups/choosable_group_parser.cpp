#include <dnd_config.hpp>

#include "choosable_group_parser.hpp"

#include <cassert>
#include <filesystem>
#include <unordered_map>
#include <vector>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/parsing/effects_provider/choosable_parser.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/utils/string_manipulation.hpp>
#include <core/validation/effects_provider/choosable_data.hpp>

namespace dnd {

ChoosableGroupParser::ChoosableGroupParser(const std::filesystem::path& filepath) noexcept
    : FileParser(filepath, true), choosable_parser(filepath) {}

Errors ChoosableGroupParser::parse() {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(
            ParsingError::Code::INVALID_FILE_FORMAT, get_filepath(), "The choosable group json file is not an object."
        );
        return errors;
    }

    group_name = get_filepath().stem().string();
    snake_case_to_capitalized_spaced_words(group_name);

    data.reserve(json.size());
    for (auto& [feature_name, feature_json] : json.items()) {
        if (!feature_json.is_object()) {
            errors.add_parsing_error(
                ParsingError::Code::INVALID_FILE_FORMAT, get_filepath(),
                fmt::format("The feature '{}' is not an object.", feature_name)
            );
            continue;
        }
        Errors feature_errors;
        ChoosableData& feature_data = data.emplace_back();
        feature_data.name = feature_name;
        feature_errors += choosable_parser.parse_into(std::move(feature_json), feature_data);
        if (!feature_errors.ok()) {
            data.pop_back();
        }
        errors += std::move(feature_errors);
    }
    choosables_in_file = data.size();
    return errors;
}

Errors ChoosableGroupParser::validate(const Content& content) const {
    Errors errors;
    assert(choosables_in_file == data.size());
    feature_data_valid.resize(choosables_in_file, false);
    for (size_t i = 0; i < choosables_in_file; ++i) {
        Errors validation_errors = data[i].validate();
        validation_errors += data[i].validate_relations(content);
        feature_data_valid[i] = validation_errors.ok();
        errors += std::move(validation_errors);
    }
    return errors;
}

void ChoosableGroupParser::save_result(Content& content) {
    for (size_t i = 0; i < data.size(); ++i) {
        if (feature_data_valid[i]) {
            snake_case_to_capitalized_spaced_words(data[i].type);
            content.add_choosable(Choosable::create(std::move(data[i]), content));
        }
    }
}

} // namespace dnd
