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
#include <core/validation/spell/spell_data.hpp>
#include <core/visitors/content/content_visitor.hpp>

namespace dnd {

CreateResult<Spell> Spell::create(Data&& data) {
    Errors errors = data.validate_nonrecursively();
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
        std::move(data.name), std::move(data.description), std::move(data.source_path), std::move(components),
        std::move(type), std::move(data.casting_time), std::move(data.range), std::move(data.duration),
        std::move(data.classes)
    ));
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
