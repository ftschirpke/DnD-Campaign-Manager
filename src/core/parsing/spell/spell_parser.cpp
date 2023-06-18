#include <dnd_config.hpp>

#include "spell_parser.hpp"

#include <fmt/format.h>
#include <nlohmann/json.hpp>

#include <core/controllers/content_holder.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/models/spell/spell.hpp>
#include <core/validation/spell/spell_data.hpp>

dnd::Errors dnd::SpellParser::parse() {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(
            ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(), "The spell file is not a json object"
        );
        return errors;
    }
    spell_data.reserve(json.size());
    for (auto& [spell_name, spell_json] : json.items()) {
        if (!spell_json.is_object()) {
            errors.add_parsing_error(
                ParsingErrorCode::INVALID_FILE_FORMAT, get_filepath(),
                fmt::format("The spell '{}' is not a json object", spell_name)
            );
            continue;
        }
        Errors spell_errors;
        SpellData data;
        data.name = spell_name;
        data.source_path = get_filepath();
        spell_errors += parse_required_attribute(spell_json, "description", data.description);
        spell_errors += parse_required_attribute(spell_json, "components", data.components_data.str);
        spell_errors += parse_required_attribute(spell_json, "level_type", data.type_data.str);
        spell_errors += parse_required_attribute(spell_json, "casting_time", data.casting_time);
        spell_errors += parse_required_attribute(spell_json, "range", data.range);
        spell_errors += parse_required_attribute(spell_json, "duration", data.duration);
        spell_errors += parse_required_attribute(spell_json, "classes", data.classes);
        if (spell_errors.ok()) {
            spell_data.emplace_back(std::move(data));
        }
        errors += std::move(spell_errors);
    }
    spells_in_file = spell_data.size();
    return errors;
}

dnd::Errors dnd::SpellParser::validate(const dnd::ContentHolder& content) const {
    Errors errors;
    assert(spells_in_file == spell_data.size());
    spell_data_valid.resize(spells_in_file, false);
    for (size_t i = 0; i < spells_in_file; ++i) {
        Errors validation_errors = spell_data[i].validate();
        validation_errors += spell_data[i].validate_relations(content);
        spell_data_valid[i] = validation_errors.ok();
        errors += std::move(validation_errors);
    }
    return errors;
}

void dnd::SpellParser::save_result(dnd::ContentHolder& content) {
    assert(spells_in_file == spell_data_valid.size());
    assert(spells_in_file == spell_data.size());
    for (size_t i = 0; i < spells_in_file; ++i) {
        if (spell_data_valid[i]) {
            content.spells.add(Spell::create(std::move(spell_data[i])));
        }
    }
}
