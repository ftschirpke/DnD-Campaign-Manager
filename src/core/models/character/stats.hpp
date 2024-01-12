#ifndef STATS_HPP_
#define STATS_HPP_

#include <dnd_config.hpp>

#include <string>
#include <unordered_map>
#include <vector>

#include <core/models/character/ability_scores.hpp>
#include <core/models/effects/stat_change/stat_change.hpp>
#include <core/utils/types.hpp>

namespace dnd {

class Stats {
public:
    static Stats create_default() noexcept;
    static Stats create_from_base_scores_and_stat_changes(
        const AbilityScores& base_ability_scores, std::vector<CRef<StatChange>> stat_changes
    ) noexcept;
    Stats() noexcept;

    bool is_complete() const noexcept;
private:
    void derive_values_from_ability_scores() noexcept;
    void calculate_ability_modifiers() noexcept;
    void calculate_skill_modifiers() noexcept;

    std::unordered_map<std::string, const int> constant_values;
    std::unordered_map<std::string, int> mutable_values;
    std::unordered_map<std::string, int> implied_values;
};

} // namespace dnd

#endif // STATS_HPP_
