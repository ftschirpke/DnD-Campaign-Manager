#ifndef SPECIES_VALIDATION_HPP_
#define SPECIES_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <core/errors/errors.hpp>
#include <core/models/species/species.hpp>

namespace dnd {

Errors validate_species_nonrecursively_for_content(const Species::Data& data, const Content& content);
Errors validate_species_recursively_for_content(const Species::Data& data, const Content& content);

} // namespace dnd

#endif // SPECIES_VALIDATION_HPP_
