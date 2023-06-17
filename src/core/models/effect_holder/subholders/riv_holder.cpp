#include <dnd_config.hpp>

#include "riv_holder.hpp"

#include <iterator>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include <fmt/format.h>

#include <core/controllers/content_holder.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/validation/effect_holder/subholders/riv_holder_data.hpp>

static std::vector<std::string> set_to_vector(std::set<std::string>&& set) {
    std::vector<std::string> vector;
    vector.reserve(set.size());
    vector.insert(vector.end(), std::make_move_iterator(set.begin()), std::make_move_iterator(set.end()));
    return vector;
}

dnd::RIVHolder dnd::RIVHolder::create(dnd::RIVHolderData&& data, const dnd::ContentHolder& content) {
    if (!data.validate().ok()) {
        throw dnd::invalid_data("Cannot create RIVHolder from invalid data.");
    }
    if (!data.validate_relations(content).ok()) {
        throw dnd::invalid_data("RIVHolderData is incompatible with the given content.");
    }
    return RIVHolder(
        set_to_vector(std::move(data.damage_resistances)), set_to_vector(std::move(data.damage_immunities)),
        set_to_vector(std::move(data.damage_vulnerabilities)), set_to_vector(std::move(data.condition_immunities))
    );
}

bool dnd::RIVHolder::empty() const {
    return damage_resistances.empty() && damage_immunities.empty() && damage_vulnerabilities.empty()
           && condition_immunities.empty();
}

const std::vector<std::string>& dnd::RIVHolder::get_damage_resistances() const noexcept { return damage_resistances; }

const std::vector<std::string>& dnd::RIVHolder::get_damage_immunities() const noexcept { return damage_immunities; }

const std::vector<std::string>& dnd::RIVHolder::get_damage_vulnerabilities() const noexcept {
    return damage_vulnerabilities;
}

const std::vector<std::string>& dnd::RIVHolder::get_condition_immunities() const noexcept {
    return condition_immunities;
}

dnd::RIVHolder::RIVHolder(
    std::vector<std::string>&& damage_resistances, std::vector<std::string>&& damage_immunities,
    std::vector<std::string>&& damage_vulnerabilities, std::vector<std::string>&& condition_immunities
) noexcept
    : damage_resistances(std::move(damage_resistances)), damage_immunities(std::move(damage_immunities)),
      damage_vulnerabilities(std::move(damage_vulnerabilities)), condition_immunities(std::move(condition_immunities)) {
}
