#ifndef SUBCLASS_HPP_
#define SUBCLASS_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <core/models/class/class.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/effects_provider/class_feature.hpp>
#include <core/models/source_info.hpp>
#include <core/models/spellcasting/spellcasting.hpp>
#include <core/validation/subclass/subclass_data.hpp>

namespace dnd {

class Content;
class ContentVisitor;

class Subclass : public ContentPiece {
public:
    /**
     * @brief Constructs a character subclass from the given data and content
     * @param data the data to construct the character subclass from
     * @param content the content to use for the construction
     * @return the constructed character subclass
     * @throws dnd::invalid_data if the given data is invalid or is incompatible with the given content
     */
    static Subclass create(SubclassData&& data, const Content& content);

    Subclass(const Subclass&) = delete;
    Subclass& operator=(const Subclass&) = delete;
    Subclass(Subclass&&) = default;
    Subclass& operator=(Subclass&&) = default;

    const std::string& get_name() const noexcept override;
    const std::string& get_description() const noexcept override;
    const SourceInfo& get_source_info() const noexcept override;
    const std::vector<ClassFeature>& get_features() const noexcept;
    bool has_spellcasting() const noexcept;
    const Spellcasting* get_spellcasting() const noexcept;
    const Class* get_class() const noexcept;

    virtual void accept_visitor(ContentVisitor& visitor) const override final;
private:
    Subclass(
        std::string&& name, std::string&& description, std::filesystem::path&& source_path,
        std::vector<ClassFeature>&& features, const Class* cls, std::unique_ptr<Spellcasting>&& spellcasting = nullptr
    ) noexcept;

    std::string name;
    std::string description;
    SourceInfo source_info;
    std::vector<ClassFeature> features;
    const Class* cls;
    std::unique_ptr<Spellcasting> spellcasting;
};

} // namespace dnd

#endif // SUBCLASS_HPP_
