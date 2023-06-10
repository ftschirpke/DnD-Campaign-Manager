#include <dnd_config.hpp>

#include "extra_spells_holder.hpp"

#include <set>
#include <utility>
#include <vector>

#include <core/controllers/content_holder.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/spell/spell.hpp>
#include <core/validation/effect_holder/subholders/extra_spells_holder_data.hpp>

static std::vector<const dnd::Spell*> find_spells_in_content(
    const std::set<std::string>& spell_names, const dnd::ContentHolder* content
) {
    std::vector<const dnd::Spell*> spells;
    spells.reserve(spell_names.size());
    for (const auto& spell_name : spell_names) {
        spells.push_back(&content->spells.get(spell_name));
    }
    return spells;
}

dnd::ExtraSpellsHolder dnd::ExtraSpellsHolder::create(
    dnd::ExtraSpellsHolderData&& data, const dnd::ContentHolder* content
) {
    if (!data.validate().ok()) {
        throw dnd::invalid_data("Cannot create ExtraSpellsHolderData from invalid data.");
    }
    if (!data.validate_relations(content).ok()) {
        throw dnd::invalid_data("ExtraSpellsHolderData is incompatible with the given content.");
    }
    std::vector<const dnd::Spell*> free_cantrips = find_spells_in_content(data.free_cantrips, content);
    std::vector<const dnd::Spell*> at_will = find_spells_in_content(data.at_will, content);
    std::vector<const dnd::Spell*> innate = find_spells_in_content(data.innate, content);
    std::vector<const dnd::Spell*> free_once_a_day = find_spells_in_content(data.free_once_a_day, content);
    std::vector<const dnd::Spell*> spells_known = find_spells_in_content(data.spells_known, content);
    std::vector<const dnd::Spell*> spells_known_included = find_spells_in_content(data.spells_known_included, content);
    std::vector<const dnd::Spell*> added_to_spell_list = find_spells_in_content(data.added_to_spell_list, content);
    return ExtraSpellsHolder(
        std::move(free_cantrips), std::move(at_will), std::move(innate), std::move(free_once_a_day),
        std::move(spells_known), std::move(spells_known_included), std::move(added_to_spell_list)
    );
}
