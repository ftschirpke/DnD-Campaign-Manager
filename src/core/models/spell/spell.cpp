#include <dnd_config.hpp>

#include "core/utils/data_result.hpp"
#include "spell.hpp"

#include <filesystem>
#include <set>
#include <string>
#include <utility>

#include <core/exceptions/validation_exceptions.hpp>
#include <core/models/content_piece.hpp>
#include <core/models/spell/spell_components.hpp>
#include <core/models/spell/spell_type.hpp>
#include <core/validation/spell/spell_validation.hpp>
#include <core/visitors/content/content_visitor.hpp>

namespace dnd {

CreateResult<Spell> Spell::create(Data&& data) {
    Errors errors = validate_spell_nonrecursively(data);
    if (!errors.ok()) {
        return InvalidCreate<Spell>(std::move(data), std::move(errors));
    }

    CreateResult<SpellComponents> components_result = SpellComponents::create(std::move(data.components_data));
    if (!components_result.is_valid()) {
        auto [_, sub_errors] = components_result.data_and_errors();
        return InvalidCreate<Spell>(std::move(data), std::move(sub_errors));
    }
    SpellComponents components = components_result.value();

    CreateResult<SpellType> type_result = SpellType::create(std::move(data.type_data));
    if (!type_result.is_valid()) {
        auto [_, sub_errors] = type_result.data_and_errors();
        return InvalidCreate<Spell>(std::move(data), std::move(sub_errors));
    }
    SpellType type = type_result.value();

    return ValidCreate(Spell(
        std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(data.source_name),
        std::move(components), std::move(type), data.concentration, std::move(data.casting_time), std::move(data.range),
        std::move(data.duration), std::move(data.classes)
    ));
}

const std::string& Spell::get_name() const { return name; }

const std::string& Spell::get_description() const { return description; }

const SourceInfo& Spell::get_source_info() const { return source_info; }

const SpellComponents& Spell::get_components() const { return components; }

const SpellType& Spell::get_type() const { return type; }

bool Spell::requires_concentration() const { return concentration; }

const std::string& Spell::get_casting_time() const { return casting_time; }

const std::string& Spell::get_range() const { return range; }

const std::string& Spell::get_duration() const { return duration; }

const std::set<std::string>& Spell::get_classes() const { return classes; }

void Spell::accept_visitor(ContentVisitor& visitor) const { visitor(*this); }

Spell::Spell(
    std::string&& name, std::string&& description, std::filesystem::path&& source_path, std::string&& source_name,
    SpellComponents&& components, SpellType&& type, bool concentration, std::string&& casting_time, std::string&& range,
    std::string&& duration, std::set<std::string>&& classes
)
    : name(std::move(name)), description(std::move(description)),
      source_info({.path = std::move(source_path), .name = std::move(source_name)}), components(std::move(components)),
      type(std::move(type)), concentration(concentration), casting_time(std::move(casting_time)),
      range(std::move(range)), duration(std::move(duration)), classes(std::move(classes)) {}

} // namespace dnd
