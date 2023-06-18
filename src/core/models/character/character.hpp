#ifndef CHARACTER_HPP_
#define CHARACTER_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <string>

#include <core/models/character/ability_scores.hpp>
#include <core/models/character/character_basis.hpp>
#include <core/models/character/decision/decision.hpp>
#include <core/models/character/progression.hpp>
#include <core/models/character_class/character_class.hpp>
#include <core/models/character_race/character_race.hpp>
#include <core/models/character_subclass/character_subclass.hpp>
#include <core/models/character_subrace/character_subrace.hpp>
#include <core/models/feature/feature.hpp>
#include <core/models/feature_holder/feature_holder.hpp>
#include <core/validation/character/character_data.hpp>

namespace dnd {

class ContentHolder;
class ContentVisitor;

class Character : public FeatureHolder {
public:
    /**
     * @brief Constructs a character from the given data and content
     * @param data the data to construct the character from
     * @param content the content to use for the construction
     * @return the constructed character
     * @throws dnd::invalid_data if the given data is invalid or is incompatible with the given content
     */
    static Character create(CharacterData&& data, const ContentHolder& content);

    Character(const Character&) = delete;
    Character& operator=(const Character&) = delete;
    Character(Character&&) = default;
    Character& operator=(Character&&) = default;

    const AbilityScores& get_base_ability_scores() const noexcept;
    const CharacterBasis& get_basis() const noexcept;
    const Progression& get_progression() const noexcept;

    /**
     * @brief Accepts a visitor
     * @param visitor pointer to the visitor
     */
    virtual void accept(ContentVisitor* visitor) const override final;
private:
    Character(
        std::string&& name, std::string&& description, std::filesystem::path&& source_path,
        std::vector<Feature>&& features, AbilityScores&& base_ability_scores, CharacterBasis&& basis,
        Progression&& progression, std::vector<Decision>&& decisions
    ) noexcept;

    AbilityScores base_ability_scores;
    CharacterBasis basis;
    Progression progression;
    std::vector<Decision> decisions;
};

} // namespace dnd

#endif // CHARACTER_HPP_