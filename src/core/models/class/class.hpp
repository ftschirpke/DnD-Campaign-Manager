#ifndef CLASS_HPP_
#define CLASS_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <core/basic_mechanics/dice.hpp>
#include <core/models/class/important_levels.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/effects_provider/class_feature.hpp>
#include <core/models/source_info.hpp>
#include <core/models/spellcasting/spellcasting.hpp>
#include <core/validation/class/class_data.hpp>

namespace dnd {

class Content;
class ContentVisitor;

class Class : public ContentPiece {
public:
    using Data = ClassData;

    /**
     * @brief Constructs a character class from the given data and content
     * @param data the data to construct the character class from
     * @param content the content to use for the construction
     * @return the constructed character class
     * @throws invalid_data if the given data is invalid or is incompatible with the given content
     */
    static Class create_for(Data&& data, const Content& content);

    Class(const Class&) = delete;
    Class& operator=(const Class&) = delete;
    Class(Class&&) = default;
    Class& operator=(Class&&) = default;

    const std::string& get_name() const noexcept override;
    const std::string& get_description() const noexcept override;
    const SourceInfo& get_source_info() const noexcept override;
    const std::vector<ClassFeature>& get_features() const noexcept;
    bool has_spellcasting() const noexcept;
    const Spellcasting* get_spellcasting() const noexcept;
    const ClassFeature* get_subclass_feature() const noexcept;
    const Dice& get_hit_dice() const noexcept;
    const ImportantLevels& get_important_levels() const noexcept;

    virtual void accept_visitor(ContentVisitor& visitor) const override final;
private:
    Class(
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

#endif // CLASS_HPP_
