#ifndef CHARACTER_CLASS_HPP_
#define CHARACTER_CLASS_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <core/basic_mechanics/dice.hpp>
#include <core/models/character_class/important_levels.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/feature/class_feature.hpp>
#include <core/models/source_info.hpp>
#include <core/models/spellcasting/spellcasting.hpp>
#include <core/validation/character_class/character_class_data.hpp>

namespace dnd {

class Content;
class ContentVisitor;

class CharacterClass : public ContentPiece {
public:
    /**
     * @brief Constructs a character class from the given data and content
     * @param data the data to construct the character class from
     * @param content the content to use for the construction
     * @return the constructed character class
     * @throws dnd::invalid_data if the given data is invalid or is incompatible with the given content
     */
    static CharacterClass create(CharacterClassData&& data, const Content& content);

    CharacterClass(const CharacterClass&) = delete;
    CharacterClass& operator=(const CharacterClass&) = delete;
    CharacterClass(CharacterClass&&) = default;
    CharacterClass& operator=(CharacterClass&&) = default;

    const std::string& get_name() const noexcept override;
    const std::string& get_description() const noexcept override;
    const SourceInfo& get_source_info() const noexcept override;
    const std::vector<ClassFeature>& get_features() const noexcept;
    bool has_spellcasting() const noexcept;
    const Spellcasting* get_spellcasting() const noexcept;
    const ClassFeature* get_subclass_feature() const noexcept;
    const Dice& get_hit_dice() const noexcept;
    const ImportantLevels& get_important_levels() const noexcept;

    /**
     * @brief Accepts a visitor
     * @param visitor reference to the visitor
     */
    virtual void accept(ContentVisitor& visitor) const override final;
private:
    CharacterClass(
        std::string&& name, std::string&& description, std::filesystem::path&& source_path,
        std::vector<ClassFeature>&& features, const ClassFeature* subclass_feature, Dice hit_dice,
        ImportantLevels&& important_levels, std::unique_ptr<Spellcasting>&& spellcasting = nullptr
    ) noexcept;

    std::string name;
    std::string description;
    SourceInfo source_info;
    std::vector<ClassFeature> features;
    std::unique_ptr<Spellcasting> spellcasting;
    const ClassFeature* subclass_feature;
    Dice hit_dice;
    ImportantLevels important_levels;
};

} // namespace dnd

#endif // CHARACTER_CLASS_HPP_
