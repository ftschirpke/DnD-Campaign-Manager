#include <dnd_config.hpp>

#include "core/parsing/class_parsing.hpp"
#include "v2_file_parser.hpp"

#include <filesystem>

#include <fmt/format.h>
#include <fmt/ranges.h>
#include <nlohmann/json.hpp>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <log.hpp>

namespace dnd {

V2FileParser::V2FileParser(const std::filesystem::path& filepath) : FileParser(filepath, true) {}

Errors V2FileParser::parse() {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(
            ParsingError::Code::INVALID_FILE_FORMAT, get_filepath(), "The v2 json is not an object."
        );
    }

    for (nlohmann::ordered_json::const_iterator it = json.cbegin(); it != json.cend(); ++it) {
        const std::string& category = it.key();
        const nlohmann::ordered_json& entry = it.value();
        if (!entry.is_array()) {
            continue;
        }

        size_t type_index = std::find(parse_types.begin(), parse_types.end(), category) - parse_types.begin();
        if (type_index >= parse_types.size()) {
            LOGWARN("Found unknown category '{}' in {}", category, get_filepath().c_str());
            continue;
        }
        ParseType parse_type = static_cast<ParseType>(type_index);

        bool is_supported;
        switch (parse_type) {
            case ParseType::class_type:
            case ParseType::classFeature_type:
                is_supported = true;
                break;
            default:
                is_supported = false;
                break;
        }
        if (!is_supported) {
            continue;
        }

        for (const nlohmann::ordered_json& element : entry) {
            if (!element.is_object()) {
                errors.add_parsing_error(
                    ParsingError::Code::INVALID_FILE_FORMAT, get_filepath(),
                    fmt::format("Element within the '{}'-entry is not an object.", category)
                );
                continue;
            }
            std::string name;
            parse_required_attribute_into(element, "name", name, get_filepath());
            LOGINFO("[{}] object '{}' found of type '{}'", get_filepath().string(), name, category);
            errors += parse_object(element, parse_type);
        }
    }
    return errors;
}

void V2FileParser::save_result(Content& content) {
    for (auto& [key, data] : parsed_data.class_data) {
        LOGINFO("Trying to add parsed class: {} {} {} {}", key, data.name, data.source_name, data.features_data.size());
        data.important_levels_data.feat_levels = {1};            // HACK: set random feat level to circumvent validation
        data.subclass_feature_name = data.features_data[0].name; // HACK: set subclass feature to circumvent validation
        data.description = "Class " + data.name;                 // HACK: set description to circumvent validation
        content.add_class_result(Class::create_for(std::move(data), content));
    }
}

Errors V2FileParser::parse_object(const nlohmann::ordered_json& obj, ParseType parse_type) {
    Errors errors;
    switch (parse_type) {
        case ParseType::class_type: {
            Class::Data result{};
            parse_class(obj, get_filepath()).move_into(result, errors);
            parsed_data.class_data.insert({result.get_key(), result});
            break;
        }
        case ParseType::classFeature_type: {
            errors += parse_class_feature(obj, get_filepath(), parsed_data.class_data);
            break;
        }
        default: {
            size_t translated_index = static_cast<size_t>(parse_type);
            const char* invalid_value = translated_index < parse_types.size() ? parse_types[translated_index]
                                                                              : "<null>";
            LOGWARN("Unexpected parsing type \"{}\"", invalid_value);
            break;
        }
    }
    return errors;
}


} // namespace dnd
