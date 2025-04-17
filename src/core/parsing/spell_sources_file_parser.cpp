#include <dnd_config.hpp>

#include "spell_sources_file_parser.hpp"

#include <filesystem>

#include <core/errors/errors.hpp>
#include <core/models/class/class.hpp>
#include <core/parsing/file_parser.hpp>

namespace dnd {

SpellSourcesFileParser::SpellSourcesFileParser(const std::filesystem::path& filepath) : FileParser(filepath, true) {}

Errors SpellSourcesFileParser::parse() {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(
            ParsingError::Code::INVALID_FILE_FORMAT, get_filepath(), "The v2 json is not an object."
        );
        return errors;
    }

    for (auto& [source_name, source_json] : json.items()) {
        if (!source_json.is_object()) {
            continue;
        }
        std::unordered_map<std::string, std::set<std::string>>& source_map = spell_classes_by_source[source_name];

        for (auto& [spell_name, spell_json] : source_json.items()) {
            if (!spell_json.is_object() || !spell_json.contains("class") || !spell_json["class"].is_array()) {
                continue;
            }
            std::set<std::string>& spell_set = source_map[spell_name];

            for (const nlohmann::ordered_json& class_entry : spell_json["class"]) {
                if (!class_entry.is_object()) {
                    continue;
                }
                std::string class_name, class_source;
                errors += parse_required_attribute_into(class_entry, "name", class_name, get_filepath());
                errors += parse_required_attribute_into(class_entry, "source", class_source, get_filepath());
                spell_set.insert(Class::key(class_name, class_source));
            }
        }
    }

    return errors;
}

void SpellSourcesFileParser::save_result(Content& content) { DND_UNUSED(content); }


} // namespace dnd
