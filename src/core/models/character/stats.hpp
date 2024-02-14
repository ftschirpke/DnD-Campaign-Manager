#ifndef STATS_HPP_
#define STATS_HPP_

#include <dnd_config.hpp>

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <tl/expected.hpp>

#include <core/models/character/ability_scores.hpp>
#include <core/models/effects/stat_change/stat_change.hpp>
#include <core/utils/types.hpp>

namespace dnd {

class Stats {
public:
    static Stats create_default();
    static tl::expected<Stats, Errors> create_from_base_scores_and_stat_changes(
        const AbilityScores& base_ability_scores, std::vector<CRef<StatChange>> stat_changes
    );

    bool is_complete() const;

    std::optional<bool> get_bool(const std::string& name) const;
    std::optional<int> get_int(const std::string& name) const;
    std::optional<float> get_float(const std::string& name) const;

    std::optional<int> get_raw(const std::string& name) const;
    std::optional<Ref<int>> get_raw_mut(const std::string& name);
    Ref<int> get_raw_mut_or_insert(const std::string& name);

    int get_ability_score(Ability ability) const;
    int get_ability_max_score(Ability ability) const;
    int get_ability_modifier(Ability ability) const;
    int get_ability_save_modifier(Ability ability) const;
private:
    Stats();

    void check_maximum_ability_scores();
    void calculate_ability_modifiers();
    void calculate_ability_save_modifiers();
    void calculate_skill_modifiers();

    std::unordered_map<std::string, const int> constant_values;
    std::unordered_map<std::string, int> mutable_values;
    std::unordered_map<std::string, int> implied_values;
};

} // namespace dnd

#endif // STATS_HPP_
