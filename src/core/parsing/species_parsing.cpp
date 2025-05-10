#include <dnd_config.hpp>

#include "species_parsing.hpp"

#include <filesystem>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/models/species/species.hpp>
#include <core/models/subspecies/subspecies.hpp>
#include <core/parsing/parser.hpp>
#include <core/text/text.hpp>

namespace dnd {

WithErrors<Species::Data> parse_species(const nlohmann::ordered_json& obj, const std::filesystem::path& filepath) {
    WithErrors<Species::Data> result;
    Species::Data& species_data = result.value;
    Errors& errors = result.errors;

    species_data.source_path = filepath;
    errors += parse_required_attribute_into(obj, "name", species_data.name, filepath);
    errors += parse_required_attribute_into(obj, "source", species_data.source_name, filepath);
    if (obj.contains("entries")) {
        errors += write_formatted_text_into(obj, species_data.description, filepath);
    } else {
        species_data.description = Text::simple("<empty description>");
    }

    return result;
}

WithErrors<Subspecies::Data> parse_subspecies(
    const nlohmann::ordered_json& obj, const std::filesystem::path& filepath
) {
    WithErrors<Subspecies::Data> result;
    Subspecies::Data& subspecies_data = result.value;
    Errors& errors = result.errors;

    subspecies_data.source_path = filepath;
    errors += parse_required_attribute_into(obj, "name", subspecies_data.name, filepath);
    errors += parse_required_attribute_into(obj, "source", subspecies_data.source_name, filepath);

    std::string species_name, species_source_name;
    errors += parse_required_attribute_into(obj, "raceName", species_name, filepath);
    errors += parse_required_attribute_into(obj, "raceSource", species_source_name, filepath);
    subspecies_data.species_key = Species::key(species_name, species_source_name);

    return result;
}

} // namespace dnd
