#ifndef SPECIES_PARSING_HPP_
#define SPECIES_PARSING_HPP_

#include <dnd_config.hpp>

#include <filesystem>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/models/species/species.hpp>
#include <core/models/subspecies/subspecies.hpp>

namespace dnd {

WithErrors<Species::Data> parse_species(const nlohmann::ordered_json& obj, const std::filesystem::path& filepath);
WithErrors<Subspecies::Data> parse_subspecies(const nlohmann::ordered_json& obj, const std::filesystem::path& filepath);


} // namespace dnd

#endif // SPECIES_PARSING_HPP_
