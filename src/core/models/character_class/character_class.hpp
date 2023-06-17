#ifndef CHARACTER_CLASS_HPP_
#define CHARACTER_CLASS_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <string>
#include <vector>

#include <core/basic_mechanics/dice.hpp>
#include <core/models/character_class/important_levels.hpp>
#include <core/models/feature/feature.hpp>
#include <core/models/feature_holder/feature_holder.hpp>
#include <core/validation/character_class/character_class_data.hpp>

namespace dnd {

class ContentHolder;
class ContentVisitor;

class CharacterClass : public FeatureHolder {
public:
    /**
     * @brief Constructs a character class from the given data and content
     * @param data the data to construct the character class from
     * @param content the content to use for the construction
     * @return the constructed character class
     * @throws dnd::invalid_data if the given data is invalid or is incompatible with the given content
     */
    static CharacterClass create(CharacterClassData&& data, const ContentHolder& content);

    CharacterClass(const CharacterClass&) = delete;
    CharacterClass& operator=(const CharacterClass&) = delete;
    CharacterClass(CharacterClass&&) = default;
    CharacterClass& operator=(CharacterClass&&) = default;

    const Feature* get_subclass_feature() const noexcept;
    const Dice& get_hit_dice() const noexcept;
    const ImportantLevels& get_important_levels() const noexcept;

    /**
     * @brief Accepts a visitor
     * @param visitor pointer to the visitor
     */
    virtual void accept(ContentVisitor* visitor) const override final;
private:
    CharacterClass(
        std::string&& name, std::string&& description, std::filesystem::path&& source_path,
        std::vector<Feature>&& features, const Feature* subclass_feature, Dice hit_dice,
        ImportantLevels&& important_levels
    ) noexcept;

    const Feature* subclass_feature;
    Dice hit_dice;
    ImportantLevels important_levels;
};

} // namespace dnd

#endif // CHARACTER_CLASS_HPP_
