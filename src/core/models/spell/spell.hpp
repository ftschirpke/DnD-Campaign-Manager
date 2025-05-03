#ifndef SPELL_HPP_
#define SPELL_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <filesystem>
#include <set>
#include <string>

#include <core/models/content_piece.hpp>
#include <core/models/source_info.hpp>
#include <core/models/spell/spell_components.hpp>
#include <core/models/spell/spell_type.hpp>
#include <core/text/text.hpp>

namespace dnd {

class ContentVisitor;

class Spell : public ContentPiece {
public:
    struct Data;

    static CreateResult<Spell> create(Data&& spell_data);

    const std::string& get_name() const override;
    const Text& get_description() const override;
    const SourceInfo& get_source_info() const override;
    const SpellComponents& get_components() const;
    const SpellType& get_type() const;
    bool requires_concentration() const;
    const std::string& get_casting_time() const;
    const std::string& get_range() const;
    const std::string& get_duration() const;
    const std::set<std::string>& get_classes() const;

    virtual void accept_visitor(ContentVisitor& visitor) const override final;
private:
    Spell(
        std::string&& name, Text&& description, std::filesystem::path&& source_path, std::string&& source_name,
        SpellComponents&& components, SpellType&& type, bool concentration, std::string&& casting_time,
        std::string&& range, std::string&& duration, std::set<std::string>&& classes
    );

    std::string name;
    Text description;
    SourceInfo source_info;
    SpellComponents components;
    SpellType type;
    bool concentration;
    std::string casting_time;
    std::string range;
    std::string duration;
    std::set<std::string> classes;
};

struct Spell::Data : public ValidationData {
    std::strong_ordering operator<=>(const Data&) const = default;

    SpellComponents::Data components_data;
    SpellType::Data type_data;

    bool concentration;
    std::string casting_time;
    std::string range;
    std::string duration;
    std::set<std::string> classes;
};

} // namespace dnd

#endif // SPELL_HPP_
