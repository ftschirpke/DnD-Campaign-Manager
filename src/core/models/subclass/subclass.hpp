#ifndef SUBCLASS_HPP_
#define SUBCLASS_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <core/models/class/class.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/effects_provider/class_feature.hpp>
#include <core/models/source_info.hpp>
#include <core/models/spellcasting/spellcasting.hpp>
#include <core/utils/types.hpp>

namespace dnd {

class Content;
class ContentVisitor;

class Subclass : public ContentPiece {
public:
    struct Data;

    static CreateResult<Subclass> create_for(Data&& data, const Content& content);

    Subclass(const Subclass&) = delete;
    Subclass& operator=(const Subclass&) = delete;
    Subclass(Subclass&&) noexcept = default;
    Subclass& operator=(Subclass&&) noexcept = default;

    const std::string& get_name() const override;
    const std::string& get_short_name() const;
    const std::string& get_description() const override;
    const SourceInfo& get_source_info() const override;
    const std::vector<ClassFeature>& get_features() const;
    bool has_spellcasting() const;
    const Spellcasting* get_spellcasting() const;
    CRef<Class> get_class() const;
    std::string get_key() const override;

    virtual void accept_visitor(ContentVisitor& visitor) const override final;
private:
    Subclass(
        std::string&& name, std::string&& description, std::filesystem::path&& source_path, std::string&& source_name,
        std::string&& short_name, std::vector<ClassFeature>&& features, CRef<Class> cls,
        std::unique_ptr<Spellcasting>&& spellcasting = nullptr
    );

    std::string name;
    std::string description;
    SourceInfo source_info;
    std::string short_name;
    std::vector<ClassFeature> features;
    CRef<Class> cls;
    std::unique_ptr<Spellcasting> spellcasting;
};

struct Subclass::Data : public ValidationData {
    std::strong_ordering operator<=>(const Data&) const = default;
    std::string get_key() const override;

    std::string short_name;
    Spellcasting::Data spellcasting_data;
    std::vector<ClassFeature::Data> features_data;
    std::string class_key;
};

} // namespace dnd

#endif // SUBCLASS_HPP_
