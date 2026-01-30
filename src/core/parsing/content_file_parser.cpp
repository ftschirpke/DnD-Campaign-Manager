#include <dnd_config.hpp>

#include "content_file_parser.hpp"

#include <filesystem>

#include <fmt/format.h>
#include <fmt/ranges.h>
#include <nlohmann/json.hpp>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/models/character/character.hpp>
#include <core/models/class/class.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/models/species/species.hpp>
#include <core/models/subclass/subclass.hpp>
#include <core/models/subspecies/subspecies.hpp>
#include <core/parsing/character_parsing.hpp>
#include <core/parsing/choosable_parsing.hpp>
#include <core/parsing/class_parsing.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/parsing/species_parsing.hpp>
#include <log.hpp>

namespace dnd {

ContentFileParser::ContentFileParser(const std::filesystem::path& filepath)
    : FileParser(filepath, true), foundry_parser(filepath) {}

Errors ContentFileParser::parse() {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(ParsingError::Code::INVALID_FILE_FORMAT, get_filepath(), "The json is not an object.");
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
            case ParseType::character_type:
            case ParseType::feat_type:
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
            errors += parse_object(element, parse_type);
        }
    }
    return errors;
}

void ContentFileParser::save_result(Content& content) {
    for (auto& [key, data] : parsed_data.class_data) {
        data.important_levels_data.feat_levels = {1};            // HACK: set random feat level to circumvent validation
        data.subclass_feature_name = data.features_data[0].name; // HACK: set subclass feature to circumvent validation
        data.description = Text::simple("Class " + data.name);   // HACK: set description to circumvent validation
        content.add_class_result(Class::create_for(std::move(data), content));
    }
    for (auto& [key, data] : parsed_data.subclass_data) {
        data.description = Text::simple("Subclass " + data.name); // HACK: set description to circumvent validation
        content.add_subclass_result(Subclass::create_for(std::move(data), content));
    }
    for (auto& [key, data] : parsed_data.species_data) {
        content.add_species_result(Species::create_for(std::move(data), content));
    }
    for (auto& [key, data] : parsed_data.subspecies_data) {
        content.add_subspecies_result(Subspecies::create_for(std::move(data), content));
    }
    for (auto& [key, data] : parsed_data.character_data) {
        content.add_character_result(Character::create_for(std::move(data), content));
    }
    for (auto& [key, data] : parsed_data.choosable_data) {
        content.add_choosable_result(Choosable::create_for(std::move(data), content));
    }
}

Errors ContentFileParser::parse_object(const nlohmann::ordered_json& obj, ParseType parse_type) {
    Errors errors;
    switch (parse_type) {
        case ParseType::class_type: {
            Class::Data result{};
            parse_class(obj, get_filepath(), foundry_parser).move_into(result, errors);
            parsed_data.class_data.insert({result.get_key(), result});
            break;
        }
        case ParseType::classFeature_type: {
            errors += parse_class_feature(obj, get_filepath(), parsed_data.class_data, foundry_parser);
            break;
        }
        case ParseType::subclass_type: {
            Subclass::Data result{};
            parse_subclass(obj, get_filepath(), foundry_parser).move_into(result, errors);
            parsed_data.subclass_data.insert({result.get_key(), result});
            break;
        }
        case ParseType::subclassFeature_type: {
            errors += parse_subclass_feature(obj, get_filepath(), parsed_data.subclass_data, foundry_parser);
            break;
        }
        case ParseType::race_type: {
            Species::Data result{};
            parse_species(obj, get_filepath(), foundry_parser).move_into(result, errors);
            parsed_data.species_data.insert({result.get_key(), result});
            break;
        }
        case ParseType::subrace_type: {
            if (!obj.contains("name")) { // empty subspecies
                break;
            }
            Subspecies::Data result{};
            parse_subspecies(obj, get_filepath(), foundry_parser).move_into(result, errors);
            parsed_data.subspecies_data.insert({result.get_key(), result});
            break;
        }
        case ParseType::character_type: {
            Character::Data result{};
            parse_character(obj, get_filepath(), foundry_parser).move_into(result, errors);
            parsed_data.character_data.insert({result.get_key(), result});
            break;
        }
        case ParseType::feat_type: {
            Choosable::Data result{};
            parse_feat(obj, get_filepath(), foundry_parser).move_into(result, errors);
            parsed_data.choosable_data.insert({result.get_key(), result});
            break;
        }
        default: {
            size_t translated_index = static_cast<size_t>(parse_type);
            const char* invalid_value = translated_index < parse_types.size() ? parse_types[translated_index]
                                                                              : "<null>";
            DND_UNUSED(invalid_value);
            LOGWARN("Unexpected parsing type \"{}\"", invalid_value);
            break;
        }
    }
    return errors;
}

FoundryFileParser::FoundryFileParser(const std::filesystem::path& filepath)
    : FileParser(filepath, true) {}

Errors FoundryFileParser::parse() { return Errors(); }

void FoundryFileParser::save_result(Content& content) { DND_UNUSED(content); }

static Opt<CRef<nlohmann::json>> get_entry(const nlohmann::json& obj, std::string type_key, const std::string& key) {
    type_key[0] = std::tolower(type_key[0]);
    DND_UNUSED(obj);
    DND_UNUSED(key);
    return std::nullopt;
}

#define IMPL_GET_OBJ_KEY(C, U, j, a, p, P)                                                                             \
    Opt<CRef<nlohmann::json>> FoundryFileParser::get_##j##_entry(const std::string& key) {          \
        return get_entry(json, #C, key);                                                                               \
    }
X_CONTENT_PIECES(IMPL_GET_OBJ_KEY)
#undef IMPL_GET_OBJ_KEY

} // namespace dnd
