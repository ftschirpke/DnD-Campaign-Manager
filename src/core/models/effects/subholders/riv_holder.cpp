#include <dnd_config.hpp>

#include "riv_holder.hpp"

#include <iterator>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include <fmt/format.h>

#include <core/content.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/utils/data_result.hpp>
#include <core/validation/effects/subholders/riv_holder_data.hpp>

namespace dnd {

static std::vector<std::string> set_to_vector(std::set<std::string>&& set) {
    std::vector<std::string> vector;
    vector.reserve(set.size());
    vector.insert(vector.end(), std::make_move_iterator(set.begin()), std::make_move_iterator(set.end()));
    return vector;
}

CreateResult<RIVHolder> RIVHolder::create_for(Data&& data, const Content& content) {
    Errors errors = validate_riv_holder_for_content(data, content);
    if (!errors.ok()) {
        return InvalidCreate<RIVHolder>(std::move(data), std::move(errors));
    }
    return ValidCreate(RIVHolder(
        set_to_vector(std::move(data.damage_resistances)), set_to_vector(std::move(data.damage_immunities)),
        set_to_vector(std::move(data.damage_vulnerabilities)), set_to_vector(std::move(data.condition_immunities))
    ));
}

const std::vector<std::string>& RIVHolder::get_damage_resistances() const noexcept { return damage_resistances; }

const std::vector<std::string>& RIVHolder::get_damage_immunities() const noexcept { return damage_immunities; }

const std::vector<std::string>& RIVHolder::get_damage_vulnerabilities() const noexcept {
    return damage_vulnerabilities;
}

const std::vector<std::string>& RIVHolder::get_condition_immunities() const noexcept { return condition_immunities; }

bool RIVHolder::empty() const {
    return damage_resistances.empty() && damage_immunities.empty() && damage_vulnerabilities.empty()
           && condition_immunities.empty();
}

void RIVHolder::merge(RIVHolder&& other) {
    damage_resistances.insert(
        damage_resistances.end(), std::make_move_iterator(other.damage_resistances.begin()),
        std::make_move_iterator(other.damage_resistances.end())
    );
    damage_immunities.insert(
        damage_immunities.end(), std::make_move_iterator(other.damage_immunities.begin()),
        std::make_move_iterator(other.damage_immunities.end())
    );
    damage_vulnerabilities.insert(
        damage_vulnerabilities.end(), std::make_move_iterator(other.damage_vulnerabilities.begin()),
        std::make_move_iterator(other.damage_vulnerabilities.end())
    );
    condition_immunities.insert(
        condition_immunities.end(), std::make_move_iterator(other.condition_immunities.begin()),
        std::make_move_iterator(other.condition_immunities.end())
    );
}

RIVHolder::RIVHolder(
    std::vector<std::string>&& damage_resistances, std::vector<std::string>&& damage_immunities,
    std::vector<std::string>&& damage_vulnerabilities, std::vector<std::string>&& condition_immunities
) noexcept
    : damage_resistances(std::move(damage_resistances)), damage_immunities(std::move(damage_immunities)),
      damage_vulnerabilities(std::move(damage_vulnerabilities)), condition_immunities(std::move(condition_immunities)) {
}

} // namespace dnd
