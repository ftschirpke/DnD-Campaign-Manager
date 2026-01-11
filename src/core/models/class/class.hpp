#ifndef CLASS_HPP_
#define CLASS_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <core/basic_mechanics/dice.hpp>
#include <core/data_result.hpp>
#include <core/models/class/important_levels.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/effects_provider/class_feature.hpp>
#include <core/models/source_info.hpp>
#include <core/models/spellcasting/spellcasting.hpp>
#include <core/text/text.hpp>
#include <core/types.hpp>

namespace dnd {

class Content;
class ContentVisitor;

class Class : public ContentPiece {
public:
    struct Data;

    static CreateResult<Class> create_for(Data&& data, const Content& content);

    Class(const Class&) = delete;
    Class& operator=(const Class&) = delete;
    Class(Class&&) noexcept = default;
    Class& operator=(Class&&) noexcept = default;

    const std::string& get_name() const override;
    const Text& get_description() const override;
    const SourceInfo& get_source_info() const override;
    const std::string& get_key() const override;
    const std::vector<ClassFeature>& get_features() const;
    bool has_spellcasting() const;
    const Spellcasting* get_spellcasting() const;
    Opt<CRef<ClassFeature>> get_subclass_feature() const;
    const Dice& get_hit_dice() const;
    const ImportantLevels& get_important_levels() const;

    virtual void accept_visitor(ContentVisitor& visitor) const override final;
private:
    Class(
        std::string&& name, Text&& description, std::filesystem::path&& source_path, std::string&& source_name,
        std::string&& key, std::vector<ClassFeature>&& features, Opt<CRef<ClassFeature>> subclass_feature, Dice hit_dice,
        ImportantLevels&& important_levels, std::unique_ptr<Spellcasting>&& spellcasting = nullptr
    );

    std::string name;
    Text description;
    SourceInfo source_info;
    std::string key;
    std::vector<ClassFeature> features;
    std::unique_ptr<Spellcasting> spellcasting;
    Opt<CRef<ClassFeature>> subclass_feature;
    Dice hit_dice;
    ImportantLevels important_levels;
};

struct Class::Data : public ValidationData {
    std::strong_ordering operator<=>(const Data&) const = default;

    Spellcasting::Data spellcasting_data;
    std::vector<ClassFeature::Data> features_data;
    std::string subclass_feature_name;
    std::string hit_dice_str;
    ImportantLevels::Data important_levels_data;
};

} // namespace dnd

#endif // CLASS_HPP_
