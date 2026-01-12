#ifndef STATS_HPP_
#define STATS_HPP_

#include <dnd_config.hpp>

#include <expected>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <core/basic_mechanics/abilities.hpp>
#include <core/basic_mechanics/dice.hpp>
#include <core/basic_mechanics/skills.hpp>
#include <core/models/character/ability_scores.hpp>
#include <core/models/effects/stat_change/stat_change.hpp>
#include <core/types.hpp>

namespace dnd {

class Stats {
public:
    static Stats create_default();
    static std::expected<Stats, Errors> create(
        const AbilityScores& base_ability_scores, int proficiency_bonus, std::vector<CRef<StatChange>> stat_changes,
        Dice class_hit_dice, const std::vector<int>& hit_dice_rolls
    );

    bool is_complete() const;

    std::optional<bool> get_bool(const std::string& name) const;
    std::optional<int> get_int(const std::string& name) const;
    std::optional<float> get_float(const std::string& name) const;

    std::optional<int> get_raw(const std::string& name) const;
    std::optional<Ref<int>> get_raw_mut(const std::string& name);
    Ref<int> get_raw_mut_or_insert(const std::string& name);

    int get_current_hp() const;
    int get_maximum_hp() const;
    int get_speed() const;
    int get_armor_class() const;
    int get_initiative() const;

    int get_ability_score(Ability ability) const;
    int get_ability_max_score(Ability ability) const;
    int get_ability_modifier(Ability ability) const;
    int get_ability_save_modifier(Ability ability) const;
    int get_skill_modifier(Skill skill) const;
private:
    Stats();

    void check_maximum_ability_scores();
    void calculate_ability_modifiers();
    void calculate_ability_save_modifiers();
    void calculate_skill_modifiers();

    /* std::unordered_map<std::string, const int> constant_values; */
    std::unordered_map<std::string, int> mutable_values;
};

} // namespace dnd

#endif // STATS_HPP_
