#include <dnd_config.hpp>

#include "character_basis.hpp"

#include <functional>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/character_class/character_class.hpp>
#include <core/models/character_species/character_species.hpp>
#include <core/models/character_subclass/character_subclass.hpp>
#include <core/models/character_subspecies/character_subspecies.hpp>
#include <core/validation/character/character_basis_data.hpp>

dnd::CharacterBasis dnd::CharacterBasis::create(dnd::CharacterBasisData&& data, const dnd::Content& content) {
    if (!data.validate().ok()) {
        throw dnd::invalid_data("Cannot create CharacterBasis object from invalid data.");
    }
    if (!data.validate_relations(content).ok()) {
        throw dnd::invalid_data("CharacterBasis data is incompatible with the given content.");
    }
    const CharacterSpecies& species = content.get_character_species().get(data.species_name).value().get();
    OptCRef<CharacterSubspecies> subspecies;
    const CharacterClass& cls = content.get_character_classes().get(data.class_name).value().get();
    OptCRef<CharacterSubclass> subclass;
    if (!data.subspecies_name.empty()) {
        subspecies = content.get_character_subspecies().get(data.subspecies_name).value().get();
    }
    if (!data.subclass_name.empty()) {
        subclass = content.get_character_subclasses().get(data.subclass_name).value().get();
    }
    return CharacterBasis(species, subspecies, cls, subclass);
}

const dnd::CharacterSpecies& dnd::CharacterBasis::get_species() const noexcept { return species; }

dnd::OptCRef<dnd::CharacterSubspecies> dnd::CharacterBasis::get_subspecies() const noexcept { return subspecies; }

const dnd::CharacterClass& dnd::CharacterBasis::get_class() const noexcept { return cls; }

dnd::OptCRef<dnd::CharacterSubclass> dnd::CharacterBasis::get_subclass() const noexcept { return subclass; }

bool dnd::CharacterBasis::has_subspecies() const noexcept { return subspecies.has_value(); }

bool dnd::CharacterBasis::has_subclass() const noexcept { return subclass.has_value(); }

dnd::CharacterBasis::CharacterBasis(
    const CharacterSpecies& species, OptCRef<CharacterSubspecies> subspecies, const CharacterClass& cls,
    OptCRef<CharacterSubclass> subclass
) noexcept
    : species(std::cref(species)), subspecies(subspecies), cls(std::cref(cls)), subclass(subclass) {}
