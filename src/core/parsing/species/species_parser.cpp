#include <dnd_config.hpp>

#include "species_parser.hpp"

#include <filesystem>
#include <string>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/parsing_error.hpp>
#include <core/models/species/species.hpp>
#include <core/parsing/effects_provider/feature_parser.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/validation/species/species_data.hpp>

namespace dnd {

SpeciesParser::SpeciesParser(const std::filesystem::path& filepath) noexcept
    : FileParser(filepath, false), feature_parser(filepath), data() {}

Errors SpeciesParser::parse() {
    Errors errors;
    if (!json.is_object()) {
        errors.add_parsing_error(
            ParsingError::Code::INVALID_FILE_FORMAT, get_filepath(), "The character species json is not an object."
        );
        return errors;
    }

    errors += parse_required_attribute_into(json, "name", data.name);
    errors += parse_required_attribute_into(json, "description", data.description);
    data.source_path = get_filepath();

    errors += parse_required_attribute_into(json, "has_subspecies", data.subspecies);

    if (json.contains("features")) {
        errors += feature_parser.parse_multiple_into(std::move(json["features"]), data.features_data);
    } else {
        errors.add_parsing_error(
            ParsingError::Code::MISSING_ATTRIBUTE, get_filepath(), "Character species has no features."
        );
    }

    return errors;
}

void SpeciesParser::save_result(Content& content) {
    content.add_species_result(Species::create_for(std::move(data), content));
}

} // namespace dnd
