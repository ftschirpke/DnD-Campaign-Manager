#include <dnd_config.hpp>

#include "spell_file_parser.hpp"

#include <filesystem>
#include <unordered_map>

#include <fmt/format.h>
#include <fmt/ranges.h>
#include <nlohmann/json.hpp>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/models/spell/spell.hpp>
#include <core/parsing/content_file_parser.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/parsing/spell_parsing.hpp>
#include <core/parsing/spell_sources_file_parser.hpp>
#include <log.hpp>

namespace dnd {

SpellFileParser::SpellFileParser(
    const std::filesystem::path& filepath, Opt<CRef<std::filesystem::path>> foundry_path,
    const SpellSources& spell_sources
)
    : FileParser(filepath, true), foundry_parser(foundry_path), spell_sources(spell_sources) {}

Errors SpellFileParser::parse() {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(ParsingError::Code::INVALID_FILE_FORMAT, get_filepath(), "The json is not an object.");
    }

    errors += foundry_parser.open_json();

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

        if (parse_type != ParseType::spell_type) {
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

            Spell::Data spell_data;
            parse_spell(element, get_filepath(), foundry_parser).move_into(spell_data, errors);

            if (spell_sources.contains(spell_data.source_name)
                && spell_sources.at(spell_data.source_name).contains(spell_data.name)) {
                spell_data.classes = spell_sources.at(spell_data.source_name).at(spell_data.name);
            }

            parsed_data.insert({spell_data.get_key(), spell_data});
        }
    }
    return errors;
}

void SpellFileParser::save_result(Content& content) {
    for (auto& [key, data] : parsed_data) {
        content.add_spell_result(Spell::create(std::move(data)));
    }
}

} // namespace dnd
