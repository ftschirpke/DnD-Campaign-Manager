#include <dnd_config.hpp>

#include "v2_file_parser.hpp"

#include <filesystem>

#include <fmt/format.h>
#include <fmt/ranges.h>
#include <nlohmann/json.hpp>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/parsing/class_parsing.hpp>
#include <core/parsing/species_parsing.hpp>
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
            case ParseType::subclass_type:
            case ParseType::subclassFeature_type:
            case ParseType::race_type:
            case ParseType::subrace_type:
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
            errors += parse_object(element, parse_type);
        }
    }
    return errors;
}

void V2FileParser::save_result(Content& content) {
    for (auto& [key, data] : parsed_data.class_data) {
        data.important_levels_data.feat_levels = {1};            // HACK: set random feat level to circumvent validation
        data.subclass_feature_name = data.features_data[0].name; // HACK: set subclass feature to circumvent validation
        data.description = "Class " + data.name;                 // HACK: set description to circumvent validation
        content.add_class_result(Class::create_for(std::move(data), content));
    }
    for (auto& [key, data] : parsed_data.subclass_data) {
        data.description = "Subclass " + data.name; // HACK: set description to circumvent validation
        content.add_subclass_result(Subclass::create_for(std::move(data), content));
    }
    for (auto& [key, data] : parsed_data.species_data) {
        content.add_species_result(Species::create_for(std::move(data), content));
    }
    for (auto& [key, data] : parsed_data.subspecies_data) {
        content.add_subspecies_result(Subspecies::create_for(std::move(data), content));
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
        case ParseType::subclass_type: {
            Subclass::Data result{};
            parse_subclass(obj, get_filepath()).move_into(result, errors);
            parsed_data.subclass_data.insert({result.get_key(), result});
            break;
        }
        case ParseType::subclassFeature_type: {
            errors += parse_subclass_feature(obj, get_filepath(), parsed_data.subclass_data);
            break;
        }
        case ParseType::race_type: {
            Species::Data result{};
            parse_species(obj, get_filepath()).move_into(result, errors);
            parsed_data.species_data.insert({result.get_key(), result});
            break;
        }
        case ParseType::subrace_type: {
            if (!obj.contains("name")) { // empty subclasses
                break;
            }
            Subspecies::Data result{};
            parse_subspecies(obj, get_filepath()).move_into(result, errors);
            parsed_data.subspecies_data.insert({result.get_key(), result});
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
