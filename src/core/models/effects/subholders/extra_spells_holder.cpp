#include <dnd_config.hpp>

#include "extra_spells_holder.hpp"

#include <set>
#include <utility>
#include <vector>

#include <core/content.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/spell/spell.hpp>
#include <core/validation/effects/subholders/extra_spells_holder_data.hpp>

namespace dnd {

static std::vector<const Spell*> find_spells_in_content(
    const std::set<std::string>& spell_names, const Content& content
) {
    std::vector<const Spell*> spells;
    spells.reserve(spell_names.size());
    for (const std::string& spell_name : spell_names) {
        spells.push_back(&content.get_spells().get(spell_name).value().get());
    }
    return spells;
}

CreateResult<ExtraSpellsHolder> ExtraSpellsHolder::create_for(Data&& data, const Content& content) {
    Errors errors = validate_extra_spells_holder_for_content(data, content);
    if (!errors.ok()) {
        return InvalidCreate<ExtraSpellsHolder>(std::move(data), std::move(errors));
    }
    std::vector<const Spell*> free_cantrips = find_spells_in_content(data.free_cantrips, content);
    std::vector<const Spell*> at_will = find_spells_in_content(data.at_will, content);
    std::vector<const Spell*> innate = find_spells_in_content(data.innate, content);
    std::vector<const Spell*> free_once_a_day = find_spells_in_content(data.free_once_a_day, content);
    std::vector<const Spell*> spells_known = find_spells_in_content(data.spells_known, content);
    std::vector<const Spell*> spells_known_included = find_spells_in_content(data.spells_known_included, content);
    std::vector<const Spell*> added_to_spell_list = find_spells_in_content(data.added_to_spell_list, content);
    return ValidCreate(ExtraSpellsHolder(
        std::move(free_cantrips), std::move(at_will), std::move(innate), std::move(free_once_a_day),
        std::move(spells_known), std::move(spells_known_included), std::move(added_to_spell_list)
    ));
}

const std::vector<const Spell*>& ExtraSpellsHolder::get_free_cantrips() const noexcept { return free_cantrips; }

const std::vector<const Spell*>& ExtraSpellsHolder::get_at_will() const noexcept { return at_will; }

const std::vector<const Spell*>& ExtraSpellsHolder::get_innate() const noexcept { return innate; }

const std::vector<const Spell*>& ExtraSpellsHolder::get_free_once_a_day() const noexcept { return free_once_a_day; }

const std::vector<const Spell*>& ExtraSpellsHolder::get_spells_known() const noexcept { return spells_known; }

const std::vector<const Spell*>& ExtraSpellsHolder::get_spells_known_included() const noexcept {
    return spells_known_included;
}

const std::vector<const Spell*>& ExtraSpellsHolder::get_added_to_spell_list() const noexcept {
    return added_to_spell_list;
}

bool ExtraSpellsHolder::empty() const {
    return free_cantrips.empty() && at_will.empty() && innate.empty() && free_once_a_day.empty() && spells_known.empty()
           && spells_known_included.empty() && added_to_spell_list.empty();
}

void ExtraSpellsHolder::merge(ExtraSpellsHolder&& other) {
    free_cantrips.insert(free_cantrips.end(), other.free_cantrips.begin(), other.free_cantrips.end());
    at_will.insert(at_will.end(), other.at_will.begin(), other.at_will.end());
    innate.insert(innate.end(), other.innate.begin(), other.innate.end());
    free_once_a_day.insert(free_once_a_day.end(), other.free_once_a_day.begin(), other.free_once_a_day.end());
    spells_known.insert(spells_known.end(), other.spells_known.begin(), other.spells_known.end());
    spells_known_included.insert(
        spells_known_included.end(), other.spells_known_included.begin(), other.spells_known_included.end()
    );
    added_to_spell_list.insert(
        added_to_spell_list.end(), other.added_to_spell_list.begin(), other.added_to_spell_list.end()
    );
}

ExtraSpellsHolder::ExtraSpellsHolder(
    std::vector<const Spell*>&& free_cantrips, std::vector<const Spell*>&& at_will, std::vector<const Spell*>&& innate,
    std::vector<const Spell*>&& free_once_a_day, std::vector<const Spell*>&& spells_known,
    std::vector<const Spell*>&& spells_known_included, std::vector<const Spell*>&& added_to_spell_list
) noexcept
    : free_cantrips(std::move(free_cantrips)), at_will(std::move(at_will)), innate(std::move(innate)),
      free_once_a_day(std::move(free_once_a_day)), spells_known(std::move(spells_known)),
      spells_known_included(std::move(spells_known_included)), added_to_spell_list(std::move(added_to_spell_list)) {}

} // namespace dnd
