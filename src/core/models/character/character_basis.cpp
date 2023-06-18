#include <dnd_config.hpp>

#include "character_basis.hpp"

#include <core/controllers/content_holder.hpp>
#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/character_class/character_class.hpp>
#include <core/models/character_race/character_race.hpp>
#include <core/models/character_subclass/character_subclass.hpp>
#include <core/models/character_subrace/character_subrace.hpp>
#include <core/validation/character/character_basis_data.hpp>

dnd::CharacterBasis dnd::CharacterBasis::create(dnd::CharacterBasisData&& data, const dnd::ContentHolder& content) {
    if (!data.validate().ok()) {
        throw dnd::invalid_data("Cannot create CharacterBasis object from invalid data.");
    }
    if (!data.validate_relations(content).ok()) {
        throw dnd::invalid_data("CharacterBasis data is incompatible with the given content.");
    }
    const CharacterRace* race = &content.character_races.get(data.race_name);
    const CharacterSubrace* subrace = nullptr;
    const CharacterClass* cls = &content.character_classes.get(data.class_name);
    const CharacterSubclass* subclass = nullptr;
    if (!data.subrace_name.empty()) {
        subrace = &content.character_subraces.get(data.subrace_name);
    }
    if (!data.subclass_name.empty()) {
        subclass = &content.character_subclasses.get(data.subclass_name);
    }
    return CharacterBasis(race, subrace, cls, subclass);
}

const dnd::CharacterRace* dnd::CharacterBasis::get_race() const noexcept { return race; }

const dnd::CharacterSubrace* dnd::CharacterBasis::get_subrace() const noexcept { return subrace; }

const dnd::CharacterClass* dnd::CharacterBasis::get_class() const noexcept { return cls; }

const dnd::CharacterSubclass* dnd::CharacterBasis::get_subclass() const noexcept { return subclass; }

bool dnd::CharacterBasis::has_subrace() const noexcept { return subrace != nullptr; }

bool dnd::CharacterBasis::has_subclass() const noexcept { return subclass != nullptr; }

std::vector<const dnd::FeatureHolder*> dnd::CharacterBasis::get_feature_holders() const {
    std::vector<const FeatureHolder*> feature_holders;
    feature_holders.push_back(race);
    if (has_subrace()) {
        feature_holders.push_back(subrace);
    }
    feature_holders.push_back(cls);
    if (has_subclass()) {
        feature_holders.push_back(subclass);
    }
    return feature_holders;
}
