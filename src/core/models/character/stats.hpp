#ifndef STATS_HPP_
#define STATS_HPP_

#include <dnd_config.hpp>

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <core/models/character/ability_scores.hpp>
#include <core/models/effects/stat_change/stat_change.hpp>
#include <core/utils/types.hpp>

namespace dnd {

class Stats {
public:
    static Stats create_default();
    static Stats create_from_base_scores_and_stat_changes(
        const AbilityScores& base_ability_scores, std::vector<CRef<StatChange>> stat_changes
    );

    Stats();

    bool is_complete() const;
    std::optional<int> get(const std::string& name) const;
    int get_or_default(const std::string& name) const;
    int get_or_else(const std::string& name, int default_value) const;
    std::optional<Ref<int>> get_mut(const std::string& name);
    int& get_mut_or_default(const std::string& name);
    int& get_mut_or_else(const std::string& name, int default_value);
private:
    void derive_values_from_ability_scores();
    void calculate_ability_modifiers();
    void calculate_skill_modifiers();

    std::unordered_map<std::string, const int> constant_values;
    std::unordered_map<std::string, int> mutable_values;
    std::unordered_map<std::string, int> implied_values;
};

} // namespace dnd

#endif // STATS_HPP_
