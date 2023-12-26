#include <dnd_config.hpp>

#include "character_basis.hpp"

#include <functional>

#include <core/content.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/character_class/character_class.hpp>
#include <core/models/character_race/character_race.hpp>
#include <core/models/character_subclass/character_subclass.hpp>
#include <core/models/character_subrace/character_subrace.hpp>
#include <core/validation/character/character_basis_data.hpp>

dnd::CharacterBasis dnd::CharacterBasis::create(dnd::CharacterBasisData&& data, const dnd::Content& content) {
    if (!data.validate().ok()) {
        throw dnd::invalid_data("Cannot create CharacterBasis object from invalid data.");
    }
    if (!data.validate_relations(content).ok()) {
        throw dnd::invalid_data("CharacterBasis data is incompatible with the given content.");
    }
    const CharacterRace& race = content.get_character_races().get(data.race_name).value().get();
    OptCRef<CharacterSubrace> subrace;
    const CharacterClass& cls = content.get_character_classes().get(data.class_name).value().get();
    OptCRef<CharacterSubclass> subclass;
    if (!data.subrace_name.empty()) {
        subrace = content.get_character_subraces().get(data.subrace_name).value().get();
    }
    if (!data.subclass_name.empty()) {
        subclass = content.get_character_subclasses().get(data.subclass_name).value().get();
    }
    return CharacterBasis(race, subrace, cls, subclass);
}

const dnd::CharacterRace& dnd::CharacterBasis::get_race() const noexcept { return race; }

dnd::OptCRef<dnd::CharacterSubrace> dnd::CharacterBasis::get_subrace() const noexcept { return subrace; }

const dnd::CharacterClass& dnd::CharacterBasis::get_class() const noexcept { return cls; }

dnd::OptCRef<dnd::CharacterSubclass> dnd::CharacterBasis::get_subclass() const noexcept { return subclass; }

bool dnd::CharacterBasis::has_subrace() const noexcept { return subrace.has_value(); }

bool dnd::CharacterBasis::has_subclass() const noexcept { return subclass.has_value(); }

dnd::CharacterBasis::CharacterBasis(
    const CharacterRace& race, OptCRef<CharacterSubrace> subrace, const CharacterClass& cls,
    OptCRef<CharacterSubclass> subclass
) noexcept
    : race(std::cref(race)), subrace(subrace), cls(std::cref(cls)), subclass(subclass) {}
