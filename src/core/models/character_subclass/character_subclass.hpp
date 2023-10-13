#ifndef CHARACTER_SUBCLASS_HPP_
#define CHARACTER_SUBCLASS_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <core/models/character_class/character_class.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/feature/class_feature.hpp>
#include <core/models/source_info.hpp>
#include <core/models/spellcasting/spellcasting.hpp>
#include <core/validation/character_subclass/character_subclass_data.hpp>

namespace dnd {

class Content;
class ContentVisitor;

class CharacterSubclass : public ContentPiece {
public:
    /**
     * @brief Constructs a character subclass from the given data and content
     * @param data the data to construct the character subclass from
     * @param content the content to use for the construction
     * @return the constructed character subclass
     * @throws dnd::invalid_data if the given data is invalid or is incompatible with the given content
     */
    static CharacterSubclass create(CharacterSubclassData&& data, const Content& content);

    CharacterSubclass(const CharacterSubclass&) = delete;
    CharacterSubclass& operator=(const CharacterSubclass&) = delete;
    CharacterSubclass(CharacterSubclass&&) = default;
    CharacterSubclass& operator=(CharacterSubclass&&) = default;

    const std::string& get_name() const noexcept override;
    const std::string& get_description() const noexcept override;
    const SourceInfo& get_source_info() const noexcept override;
    const std::vector<ClassFeature>& get_features() const noexcept;
    bool has_spellcasting() const noexcept;
    const Spellcasting* get_spellcasting() const noexcept;
    const CharacterClass* get_class() const noexcept;

    /**
     * @brief Accepts a visitor
     * @param visitor reference to the visitor
     */
    virtual void accept(ContentVisitor& visitor) const override final;
private:
    CharacterSubclass(
        std::string&& name, std::string&& description, std::filesystem::path&& source_path,
        std::vector<ClassFeature>&& features, const CharacterClass* cls,
        std::unique_ptr<Spellcasting>&& spellcasting = nullptr
    ) noexcept;

    std::string name;
    std::string description;
    SourceInfo source_info;
    std::vector<ClassFeature> features;
    const CharacterClass* cls;
    std::unique_ptr<Spellcasting> spellcasting;
};

} // namespace dnd

#endif // CHARACTER_SUBCLASS_HPP_
