#include <dnd_config.hpp>

#include "spell.hpp"

#include <filesystem>
#include <set>
#include <string>
#include <utility>

#include <core/models/content_piece.hpp>
#include <core/models/spell/spell_components.hpp>
#include <core/models/spell/spell_type.hpp>
#include <core/validation/spell/spell_data.hpp>

dnd::Spell dnd::Spell::create(
    const std::string& name, const std::filesystem::path& source_path, const std::string& description,
    const std::string& components_str, const std::string& type_str, const std::string& casting_time,
    const std::string& range, const std::string& duration, const std::set<std::string>& classes
) {
    SpellData spell_data{name,         source_path, description, components_str, type_str,
                         casting_time, range,       duration,    classes};
    return create(std::move(spell_data));
}

dnd::Spell dnd::Spell::create(dnd::SpellData&& spell_data) {
    if (!spell_data.validate().ok()) {
        throw invalid_data("Spell cannot be created from invalid data.");
    }

    SpellComponents components = spell_data.create_components();
    SpellType type = spell_data.create_type();

    return Spell(
        std::move(spell_data.name), std::move(spell_data.source_path), std::move(spell_data.description),
        std::move(components), std::move(type), std::move(spell_data.casting_time), std::move(spell_data.range),
        std::move(spell_data.duration), std::move(spell_data.classes)
    );
}

void dnd::Spell::accept(dnd::ContentVisitor* visitor) const { visitor->visit(this); }

dnd::Spell::Spell(
    std::string&& name, std::filesystem::path&& source_path, std::string&& description, SpellComponents&& components,
    SpellType&& type, std::string&& casting_time, std::string&& range, std::string&& duration,
    std::set<std::string>&& classes
) noexcept
    : ContentPiece(std::move(name), std::move(source_path), std::move(description)), components(std::move(components)),
      type(std::move(type)), casting_time(std::move(casting_time)), range(std::move(range)),
      duration(std::move(duration)), classes(std::move(classes)) {}
