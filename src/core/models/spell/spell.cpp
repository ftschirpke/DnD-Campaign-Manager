#include <dnd_config.hpp>

#include "spell.hpp"

#include <filesystem>
#include <set>
#include <string>
#include <utility>

#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/spell/spell_components.hpp>
#include <core/models/spell/spell_type.hpp>
#include <core/validation/spell/spell_data.hpp>
#include <core/visitors/content/content_visitor.hpp>

namespace dnd {

Spell Spell::create(Data&& spell_data) {
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

const std::string& Spell::get_name() const noexcept { return name; }

const std::string& Spell::get_description() const noexcept { return description; }

const SourceInfo& Spell::get_source_info() const noexcept { return source_info; }

const SpellComponents& Spell::get_components() const noexcept { return components; }

const SpellType& Spell::get_type() const noexcept { return type; }

const std::string& Spell::get_casting_time() const noexcept { return casting_time; }

const std::string& Spell::get_range() const noexcept { return range; }

const std::string& Spell::get_duration() const noexcept { return duration; }

const std::set<std::string>& Spell::get_classes() const noexcept { return classes; }

void Spell::accept_visitor(ContentVisitor& visitor) const { visitor(*this); }

Spell::Spell(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path, SpellComponents&& components,
    SpellType&& type, std::string&& casting_time, std::string&& range, std::string&& duration,
    std::set<std::string>&& classes
) noexcept
    : name(std::move(name)), description(std::move(description)), source_info(std::move(source_path)),
      components(std::move(components)), type(std::move(type)), casting_time(std::move(casting_time)),
      range(std::move(range)), duration(std::move(duration)), classes(std::move(classes)) {}

} // namespace dnd
