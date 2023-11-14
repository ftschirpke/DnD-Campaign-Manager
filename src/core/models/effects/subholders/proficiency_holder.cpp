#include <dnd_config.hpp>

#include "proficiency_holder.hpp"

#include <iterator>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include <core/content.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/validation/effects/subholders/proficiency_holder_data.hpp>

static std::vector<std::string> set_to_vector(std::set<std::string>&& set) {
    std::vector<std::string> vector;
    vector.reserve(set.size());
    vector.insert(vector.end(), std::make_move_iterator(set.begin()), std::make_move_iterator(set.end()));
    return vector;
}

dnd::ProficiencyHolder dnd::ProficiencyHolder::create(ProficiencyHolderData&& data, const Content& content) {
    if (!data.validate().ok()) {
        throw dnd::invalid_data("Cannot create ProficiencyHolder from invalid data.");
    }
    if (!data.validate_relations(content).ok()) {
        throw dnd::invalid_data("ProficiencyHolderData is incompatible with the given content.");
    }
    return ProficiencyHolder(
        set_to_vector(std::move(data.armor)), set_to_vector(std::move(data.weapons)),
        set_to_vector(std::move(data.tools)), set_to_vector(std::move(data.saving_throws)),
        set_to_vector(std::move(data.skills)), set_to_vector(std::move(data.languages)),
        set_to_vector(std::move(data.senses))
    );
}

const std::vector<std::string>& dnd::ProficiencyHolder::get_armor_proficiencies() const noexcept {
    return armor_proficiencies;
}

const std::vector<std::string>& dnd::ProficiencyHolder::get_weapon_proficiencies() const noexcept {
    return weapon_proficiencies;
}

const std::vector<std::string>& dnd::ProficiencyHolder::get_tool_proficiencies() const noexcept {
    return tool_proficiencies;
}

const std::vector<std::string>& dnd::ProficiencyHolder::get_skill_proficiencies() const noexcept {
    return skill_proficiencies;
}

const std::vector<std::string>& dnd::ProficiencyHolder::get_saving_throw_proficiencies() const noexcept {
    return saving_throw_proficiencies;
}

const std::vector<std::string>& dnd::ProficiencyHolder::get_known_languages() const noexcept { return known_languages; }

const std::vector<std::string>& dnd::ProficiencyHolder::get_senses() const noexcept { return senses; }

bool dnd::ProficiencyHolder::empty() const {
    return armor_proficiencies.empty() && weapon_proficiencies.empty() && tool_proficiencies.empty()
           && skill_proficiencies.empty() && saving_throw_proficiencies.empty() && known_languages.empty()
           && senses.empty();
}

void dnd::ProficiencyHolder::merge(dnd::ProficiencyHolder&& other) {
    armor_proficiencies.insert(
        armor_proficiencies.end(), std::make_move_iterator(other.armor_proficiencies.begin()),
        std::make_move_iterator(other.armor_proficiencies.end())
    );
    weapon_proficiencies.insert(
        weapon_proficiencies.end(), std::make_move_iterator(other.weapon_proficiencies.begin()),
        std::make_move_iterator(other.weapon_proficiencies.end())
    );
    tool_proficiencies.insert(
        tool_proficiencies.end(), std::make_move_iterator(other.tool_proficiencies.begin()),
        std::make_move_iterator(other.tool_proficiencies.end())
    );
    skill_proficiencies.insert(
        skill_proficiencies.end(), std::make_move_iterator(other.skill_proficiencies.begin()),
        std::make_move_iterator(other.skill_proficiencies.end())
    );
    saving_throw_proficiencies.insert(
        saving_throw_proficiencies.end(), std::make_move_iterator(other.saving_throw_proficiencies.begin()),
        std::make_move_iterator(other.saving_throw_proficiencies.end())
    );
    known_languages.insert(
        known_languages.end(), std::make_move_iterator(other.known_languages.begin()),
        std::make_move_iterator(other.known_languages.end())
    );
    senses.insert(
        senses.end(), std::make_move_iterator(other.senses.begin()), std::make_move_iterator(other.senses.end())
    );
}

dnd::ProficiencyHolder::ProficiencyHolder(
    std::vector<std::string>&& armor, std::vector<std::string>&& weapons, std::vector<std::string>&& tools,
    std::vector<std::string>&& skills, std::vector<std::string>&& saving_throws, std::vector<std::string>&& languages,
    std::vector<std::string>&& senses
) noexcept
    : armor_proficiencies(std::move(armor)), weapon_proficiencies(std::move(weapons)),
      tool_proficiencies(std::move(tools)), skill_proficiencies(std::move(skills)),
      saving_throw_proficiencies(std::move(saving_throws)), known_languages(std::move(languages)),
      senses(std::move(senses)) {}
