#include <dnd_config.hpp>

#include "proficiency_holder.hpp"

#include <iterator>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include <core/content.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/validation/effect_holder/subholders/proficiency_holder_data.hpp>

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

const std::vector<std::string>& dnd::ProficiencyHolder::get_armor() const noexcept { return armor; }

const std::vector<std::string>& dnd::ProficiencyHolder::get_weapons() const noexcept { return weapons; }

const std::vector<std::string>& dnd::ProficiencyHolder::get_tools() const noexcept { return tools; }

const std::vector<std::string>& dnd::ProficiencyHolder::get_skills() const noexcept { return skills; }

const std::vector<std::string>& dnd::ProficiencyHolder::get_saving_throws() const noexcept { return saving_throws; }

const std::vector<std::string>& dnd::ProficiencyHolder::get_languages() const noexcept { return languages; }

const std::vector<std::string>& dnd::ProficiencyHolder::get_senses() const noexcept { return senses; }

bool dnd::ProficiencyHolder::empty() const {
    return armor.empty() && weapons.empty() && tools.empty() && skills.empty() && saving_throws.empty()
           && languages.empty() && senses.empty();
}

void dnd::ProficiencyHolder::merge(dnd::ProficiencyHolder&& other) {
    armor.insert(armor.end(), std::make_move_iterator(other.armor.begin()), std::make_move_iterator(other.armor.end()));
    weapons.insert(
        weapons.end(), std::make_move_iterator(other.weapons.begin()), std::make_move_iterator(other.weapons.end())
    );
    tools.insert(tools.end(), std::make_move_iterator(other.tools.begin()), std::make_move_iterator(other.tools.end()));
    skills.insert(
        skills.end(), std::make_move_iterator(other.skills.begin()), std::make_move_iterator(other.skills.end())
    );
    saving_throws.insert(
        saving_throws.end(), std::make_move_iterator(other.saving_throws.begin()),
        std::make_move_iterator(other.saving_throws.end())
    );
    languages.insert(
        languages.end(), std::make_move_iterator(other.languages.begin()),
        std::make_move_iterator(other.languages.end())
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
    : armor(std::move(armor)), weapons(std::move(weapons)), tools(std::move(tools)), skills(std::move(skills)),
      saving_throws(std::move(saving_throws)), languages(std::move(languages)), senses(std::move(senses)) {}
