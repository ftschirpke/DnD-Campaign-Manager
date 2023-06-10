#ifndef SPELL_HPP_
#define SPELL_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <set>
#include <string>

#include <core/models/content_piece.hpp>
#include <core/models/spell/spell_components.hpp>
#include <core/models/spell/spell_type.hpp>
#include <core/validation/spell/spell_data.hpp>

namespace dnd {

class Spell : public ContentPiece {
public:
    /**
     * @brief Constructs a spell
     * @param name the name of the spell
     * @param source_path the path to the source file
     * @param description a description of how the spell works and what is does
     * @param components_str the string representation of the spell's components
     * @param type_str the string representation of the spell's type
     * @param casting_time the casting time of the spell
     * @param range the range of the spell
     * @param duration the duration of the spell
     * @param classes the classes that can cast the spell
     * @return the constructed spell
     * @throws dnd::invalid_data if the data is invalid
     */
    static Spell create(
        const std::string& name, const std::filesystem::path& source_path, const std::string& description,
        const std::string& components_str, const std::string& type_str, const std::string& casting_time,
        const std::string& range, const std::string& duration, const std::set<std::string>& classes
    );
    /**
     * @brief Constructs a spell
     * @param spell_data the data to construct the spell from
     * @return the constructed spell
     * @throws dnd::invalid_data if the data is invalid
     */
    static Spell create(SpellData&& spell_data);

    const SpellComponents& get_components() const noexcept;
    const SpellType& get_type() const noexcept;
    const std::string& get_casting_time() const noexcept;
    const std::string& get_range() const noexcept;
    const std::string& get_duration() const noexcept;
    const std::set<std::string>& get_classes() const noexcept;

    /**
     * @brief Accepts a visitor
     * @param visitor pointer to the visitor
     */
    virtual void accept(ContentVisitor* visitor) const override final;
private:
    Spell(
        std::string&& name, std::filesystem::path&& source_path, std::string&& description,
        SpellComponents&& components, SpellType&& type, std::string&& casting_time, std::string&& range,
        std::string&& duration, std::set<std::string>&& classes
    ) noexcept;

    SpellComponents components;
    SpellType type;
    std::string casting_time;
    std::string range;
    std::string duration;
    std::set<std::string> classes;
};

} // namespace dnd

#endif // SPELL_HPP_
