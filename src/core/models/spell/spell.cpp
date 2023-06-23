#include <dnd_config.hpp>

#include "spell.hpp"

#include <filesystem>
#include <set>
#include <string>
#include <utility>

#include <core/content_visitors/content_visitor.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/spell/spell_components.hpp>
#include <core/models/spell/spell_type.hpp>
#include <core/validation/spell/spell_data.hpp>

dnd::Spell dnd::Spell::create(dnd::SpellData&& spell_data) {
    if (!spell_data.validate().ok()) {
        throw invalid_data("Cannot create Spell from invalid data.");
    }

    SpellComponents components = SpellComponents::create(std::move(spell_data.components_data));
    SpellType type = SpellType::create(std::move(spell_data.type_data));

    return Spell(
        std::move(spell_data.name), std::move(spell_data.description), std::move(spell_data.source_path),
        std::move(components), std::move(type), std::move(spell_data.casting_time), std::move(spell_data.range),
        std::move(spell_data.duration), std::move(spell_data.classes)
    );
}

const dnd::SpellComponents& dnd::Spell::get_components() const noexcept { return components; }

const dnd::SpellType& dnd::Spell::get_type() const noexcept { return type; }

const std::string& dnd::Spell::get_casting_time() const noexcept { return casting_time; }

const std::string& dnd::Spell::get_range() const noexcept { return range; }

const std::string& dnd::Spell::get_duration() const noexcept { return duration; }

const std::set<std::string>& dnd::Spell::get_classes() const noexcept { return classes; }

void dnd::Spell::accept(dnd::ContentVisitor* visitor) const { visitor->visit(this); }

dnd::Spell::Spell(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path, SpellComponents&& components,
    SpellType&& type, std::string&& casting_time, std::string&& range, std::string&& duration,
    std::set<std::string>&& classes
) noexcept
    : ContentPiece(std::move(name), std::move(description), std::move(source_path)), components(std::move(components)),
      type(std::move(type)), casting_time(std::move(casting_time)), range(std::move(range)),
      duration(std::move(duration)), classes(std::move(classes)) {}
