#ifndef CHARACTER_HPP_
#define CHARACTER_HPP_

#include "dnd_config.hpp"

#include <array>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "models/character_class.hpp"
#include "models/character_race.hpp"
#include "models/character_state.hpp"
#include "models/character_subclass.hpp"
#include "models/character_subrace.hpp"
#include "models/effect_holder/character_decision.hpp"
#include "models/effect_holder/feature.hpp"
#include "models/feature_holder.hpp"

namespace dnd {

/**
 * @brief A class representing a character
 */
class Character : public FeatureHolder {
public:
    /**
     * @brief Constructs a character with level 1 and zero XP.
     * @param name the name of the character
     * @param features a collection of character-specific features (features not given by its class or race)
     * @param base_ability_scores the 6 base values for the character's ability scores
     */
    Character(
        const std::string& name, const std::filesystem::path& source_file_path, std::vector<Feature>&& features,
        const std::array<int, 6>& base_ability_scores
    ) noexcept;
    /**
     * @brief Constructs a character with a given level and XP value
     * @param name the name of the character
     * @param features a collection of character-specific features (features not given by its class or race)
     * @param base_ability_scores the 6 base values for the character's ability scores
     * @param level the level of the character
     * @param xp the XP value of the character
     * @param hit_dice_rolls the values rolled for maxHP at each level-up using your hit dice (including level 1)
     */
    Character(
        const std::string& name, const std::filesystem::path& source_file_path, std::vector<Feature>&& features,
        const std::array<int, 6>& base_ability_scores, int level, int xp, const std::vector<int>& hit_dice_rolls
    ) noexcept;
    /**
     * @brief Returns the level of the character
     * @return the level of the character
     */
    int getLevel() const noexcept;
    /**
     * @brief Returns the XP value of the character
     * @return the XP value of the character
     */
    int getXP() const noexcept;

    // void levelUp();
    // void setLevel(int new_level);
    // void setXP(int new_xp);
    // void increaseXP(int xp_increase);
    // void decreaseXP(int xp_decrease);

    /**
     * @brief Returns the hit dice rolls of the character
     * @return the hit dice rolls of the character
     */
    const std::vector<int>& getHitDiceRolls() const noexcept;
    /**
     * @brief Determines the level for a character with a given XP value
     * @param xp the XP value of a character
     * @return the corresponding character level for this XP value
     * @throws std::invalid_argument if the XP value is smaller than zero
     */
    static int levelForXP(int xp);

    // void addHitDiceRoll(int hit_dice_roll);

    /**
     * @brief Determine the current state of the character (stats, equipment, spells, etc.)
     */
    void determineState();
    /**
     * @brief Returns all the features the character has or will have access to
     * @return all the character-specific features as well as all the features of each of its feature holders
     */
    std::vector<const Feature*> allFeatures() const;

    // TODO: should these pointers be non-const?

    // the respective minimum XP values for levels 1 through 20 (indices 0 to 19)
    static const std::array<int, 20> minxp_for_level;
    // the 6 base values for the character's ability scores
    const std::array<int, 6> base_ability_scores;
    // a pointer to the class of the character
    const CharacterClass* class_ptr;
    // a pointer to the subclass of the character, nullptr if he does not have one
    const CharacterSubclass* subclass_ptr;
    // a pointer to the race of the character
    const CharacterRace* race_ptr;
    // a pointer to the subrace of the character, nullptr if he does not have one
    const CharacterSubrace* subrace_ptr;
    // the character's decisions for certain choices he had to make for features or choosables
    std::vector<CharacterDecision> decisions;
    // the current state of the character
    CharacterState state;
protected:
    /**
     * @brief Get the values for the character that may be used during state calculation but cannot change
     * @return the character constant values for the calculation of the character's state
     */
    virtual const std::unordered_map<std::string, int> getConstants() const;
    /**
     * @brief Get the initial attribute values for the character that may be used and changed during state calculation
     * @return the initial character attribute values for the calculation of the character's state
     */
    virtual const std::unordered_map<std::string, int> getInitialAttributeValues() const;
private:
    /**
     * @brief Sets the character's level to the value that corresponds to the current XP value of the character
     */
    void updateLevel();

    // level the level of the character
    int level;
    // xp the XP value of the character
    int xp;
    // the values rolled for maxHP at each level-up using your hit dice (including level 1)
    std::vector<int> hit_dice_rolls;
};

inline Character::Character(
    const std::string& name, const std::filesystem::path& source_file_path, std::vector<Feature>&& features,
    const std::array<int, 6>& base_ability_scores
) noexcept
    : FeatureHolder(name, source_file_path, std::move(features)), base_ability_scores(base_ability_scores),
      class_ptr(nullptr), subclass_ptr(nullptr), race_ptr(nullptr), subrace_ptr(nullptr), decisions(), state(decisions),
      level(1), xp(0), hit_dice_rolls() {}


inline Character::Character(
    const std::string& name, const std::filesystem::path& source_file_path, std::vector<Feature>&& features,
    const std::array<int, 6>& base_ability_scores, int level, int xp, const std::vector<int>& hit_dice_rolls
) noexcept
    : FeatureHolder(name, source_file_path, std::move(features)), base_ability_scores(base_ability_scores),
      class_ptr(nullptr), subclass_ptr(nullptr), race_ptr(nullptr), subrace_ptr(nullptr), decisions(), state(decisions),
      level(level), xp(xp), hit_dice_rolls(hit_dice_rolls) {}

inline int Character::getLevel() const noexcept { return level; }

inline int Character::getXP() const noexcept { return xp; }

inline void Character::updateLevel() { level = levelForXP(xp); }

inline const std::vector<int>& Character::getHitDiceRolls() const noexcept { return hit_dice_rolls; }

} // namespace dnd

#endif // CHARACTER_HPP_
