#include <dnd_config.hpp>

#include "v2_file_parser.hpp"

#include <filesystem>

#include <fmt/format.h>
#include <fmt/ranges.h>
#include <nlohmann/json.hpp>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/log.hpp>

namespace dnd {

V2FileParser::V2FileParser(const std::filesystem::path& filepath) : FileParser(filepath, true) {}

Errors V2FileParser::parse() {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(
            ParsingError::Code::INVALID_FILE_FORMAT, get_filepath(), "The v2 json is not an object."
        );
    }


    for (nlohmann::ordered_json::iterator it = json.begin(); it != json.end(); ++it) {
        const std::string& category = it.key();
        nlohmann::ordered_json& entry = it.value();
        if (!entry.is_array()) {
            errors.add_parsing_error(
                ParsingError::Code::INVALID_FILE_FORMAT, get_filepath(),
                fmt::format("The top-level JSON entry with key '{}' is not an array.", category)
            );
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
                is_supported = true;
                break;
            default:
                is_supported = false;
                break;
        }
        if (!is_supported) {
            continue;
        }

        for (nlohmann::ordered_json& element : entry) {
            if (!element.is_object()) {
                errors.add_parsing_error(
                    ParsingError::Code::INVALID_FILE_FORMAT, get_filepath(),
                    fmt::format("Element within the '{}'-entry is not an object.", category)
                );
                continue;
            }
            std::string name;
            parse_required_attribute_into(element, "name", name);
            LOGINFO("[{}] object '{}' found of type '{}'", get_filepath().string(), name, category);
        }
    }
    return errors;
}

void V2FileParser::save_result(Content& content) {
    for (Class::Data& datum : parsed_data.class_data) {
        content.add_class_result(Class::create_for(std::move(datum), content));
    }
}

Errors V2FileParser::parse_object(nlohmann::ordered_json& obj, ParseType parse_type) {
    Errors errors;
    switch (parse_type) {
        case ParseType::class_type: {
            Class::Data class_data;
            class_data.source_path = get_filepath();
            errors += parse_required_attribute_into(obj, "name", class_data.name);
            errors += parse_required_attribute_into(obj, "source", class_data.source_name);
            class_data.spellcasting_data.is_spellcaster = obj.contains("spellcastingAbility");
            if (class_data.spellcasting_data.is_spellcaster) {
                errors += parse_required_attribute_into(
                    obj, "spellcastingAbility", class_data.spellcasting_data.ability
                );
                errors += parse_required_attribute_into(
                    obj, "casterProgression", class_data.spellcasting_data.preparation_spellcasting_type
                );
                errors += parse_required_attribute_into(
                    obj, "cantripProgression", class_data.spellcasting_data.cantrips_known
                );
                errors += parse_required_attribute_into(
                    obj, "spellsKnownProgression", class_data.spellcasting_data.spells_known
                );
            }
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
