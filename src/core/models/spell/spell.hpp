#ifndef SPELL_HPP_
#define SPELL_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <set>
#include <string>

#include <core/models/content_piece.hpp>
#include <core/models/source_info.hpp>
#include <core/models/spell/spell_components.hpp>
#include <core/models/spell/spell_type.hpp>
#include <core/validation/spell/spell_data.hpp>

namespace dnd {

class ContentVisitor;

class Spell : public ContentPiece {
public:
    using Data = SpellData;

    /**
     * @brief Constructs a spell
     * @param spell_data the data to construct the spell from
     * @return the constructed spell
     * @throws invalid_data if the data is invalid
     */
    static Spell create(Data&& spell_data);

    const std::string& get_name() const noexcept override;
    const std::string& get_description() const noexcept override;
    const SourceInfo& get_source_info() const noexcept override;
    const SpellComponents& get_components() const noexcept;
    const SpellType& get_type() const noexcept;
    const std::string& get_casting_time() const noexcept;
    const std::string& get_range() const noexcept;
    const std::string& get_duration() const noexcept;
    const std::set<std::string>& get_classes() const noexcept;

    virtual void accept_visitor(ContentVisitor& visitor) const override final;
private:
    Spell(
        std::string&& name, std::string&& description, std::filesystem::path&& source_path,
        SpellComponents&& components, SpellType&& type, std::string&& casting_time, std::string&& range,
        std::string&& duration, std::set<std::string>&& classes
    ) noexcept;

    std::string name;
    std::string description;
    SourceInfo source_info;
    SpellComponents components;
    SpellType type;
    std::string casting_time;
    std::string range;
    std::string duration;
    std::set<std::string> classes;
};

} // namespace dnd

#endif // SPELL_HPP_
