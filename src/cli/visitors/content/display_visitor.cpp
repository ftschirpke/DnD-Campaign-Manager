#include <dnd_config.hpp>

#include "display_visitor.hpp"

#include <cassert>
#include <memory>
#include <string>
#include <vector>

#include <fmt/format.h>

#include <cli/output/command_line_output.hpp>
#include <core/basic_mechanics/dice.hpp>
#include <core/models/character/character.hpp>
#include <core/models/class/class.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/source_info.hpp>
#include <core/models/species/species.hpp>
#include <core/models/spell/spell.hpp>
#include <core/models/subclass/subclass.hpp>
#include <core/models/subspecies/subspecies.hpp>
#include <core/output/string_formatting/formats/format.hpp>
#include <core/output/string_formatting/string_formatter.hpp>
#include <core/utils/types.hpp>
#include <core/visitors/content/content_visitor.hpp>

namespace dnd {

constexpr const char* separator = "--------------------------------------------------------------------------------";

static void display_source_info(CommandLineOutput& output, const SourceInfo& source_info) {
    output.formatted_text(
        "Source: {} / {} / {}", source_info.get_source_group_name(), source_info.get_source_type_name(),
        source_info.get_source_name()
    );
}

static void format_text(const std::string& text) {
    static DisplayFormatVisitor display_format_visitor;
    static StringFormatter string_formatter(false);
    std::vector<std::unique_ptr<Format>> formats = string_formatter.parse_formats(text);
    for (const std::unique_ptr<Format>& format : formats) {
        format->accept(display_format_visitor);
    }
}

template <typename T>
static void list_features(CommandLineOutput& output, const std::vector<T>& features) {
    if (features.empty()) {
        return;
    }
    output.text("Features:");
    for (const Feature& feature : features) {
        output.text(separator);
        output.text(feature.get_name());
        output.text("Description:");
        format_text(feature.get_description());
    }
}

void DisplayVisitor::operator()(const Character& character) {
    output.text(character.get_name());
    output.text("Type: Character");
    display_source_info(output, character.get_source_info());
    output.formatted_text("Level: {}", character.get_progression().get_level());
    output.formatted_text("XP: {}", character.get_progression().get_xp());

    const Species& species = character.get_feature_providers().get_species();
    output.formatted_text("Species: {}", species.get_name());

    OptCRef<Subspecies> subspecies = character.get_feature_providers().get_subspecies();
    if (subspecies.has_value()) {
        output.formatted_text("Subspecies: {}", subspecies.value().get().get_name());
    }

    const Class& cls = character.get_feature_providers().get_class();
    output.formatted_text("Class: {}", cls.get_name());

    OptCRef<Subclass> subclass = character.get_feature_providers().get_subclass();
    if (subclass.has_value()) {
        output.formatted_text("Subclass: {}", subclass.value().get().get_name());
    }

    list_features<Feature>(output, character.get_features());
}

void DisplayVisitor::operator()(const Class& cls) {
    output.text(cls.get_name());
    output.text("Type: Class");
    display_source_info(output, cls.get_source_info());
    output.formatted_text("Hit Die: {}", cls.get_hit_dice().to_string());

    std::string feat_level_str = fmt::format("{}", fmt::join(cls.get_important_levels().get_feat_levels(), ", "));
    output.formatted_text("Feat Levels: {}", feat_level_str);
    output.formatted_text("Subclass Level: {}", cls.get_important_levels().get_subclass_level());

    list_features<ClassFeature>(output, cls.get_features());
}

void DisplayVisitor::operator()(const Subclass& subclass) {
    output.text(subclass.get_name());
    output.text("Type: Subclass");
    display_source_info(output, subclass.get_source_info());
    output.formatted_text("Class name:", subclass.get_class().get().get_name());

    list_features<ClassFeature>(output, subclass.get_features());
}

void DisplayVisitor::operator()(const Species& species) {
    output.text(species.get_name());
    output.text("Type: Species");
    display_source_info(output, species.get_source_info());
    const char* has_subspecies_cstr = species.has_subspecies() ? "yes" : "no";
    output.formatted_text("Has Subspecies: {}", has_subspecies_cstr);

    list_features<Feature>(output, species.get_features());
}

void DisplayVisitor::operator()(const Subspecies& subspecies) {
    output.text(subspecies.get_name());
    output.text("Type: Subspecies");
    display_source_info(output, subspecies.get_source_info());
    output.formatted_text("Species name: {}", subspecies.get_species().get().get_name());

    list_features<Feature>(output, subspecies.get_features());
}

void DisplayVisitor::operator()(const Item& item) {
    output.text(item.get_name());
    output.text("Type: Item");
    display_source_info(output, item.get_source_info());
    const char* attunement = item.requires_attunement() ? "required" : "not required";
    output.formatted_text("Attunement {}", attunement);
    output.text("Description:");
    format_text(item.get_description());
    if (!item.get_cosmetic_description().empty()) {
        output.text("Cosmetic Description:");
        format_text(item.get_cosmetic_description());
    }
}

void DisplayVisitor::operator()(const Spell& spell) {
    output.text(spell.get_name());
    output.text("Type: Spell");
    display_source_info(output, spell.get_source_info());
    output.formatted_text("Spell Type: {}", spell.get_type().str());
    output.formatted_text("Casting Time: {}", spell.get_casting_time());
    output.formatted_text("Range: {}", spell.get_range());
    output.formatted_text("Components: {}", spell.get_components().str());
    output.formatted_text("Duration: {}", spell.get_duration());

    output.text("Description:");
    format_text(spell.get_description());
}

void DisplayVisitor::operator()(const Feature& feature) {
    output.text(feature.get_name());
    output.text("Type: Feature");
    display_source_info(output, feature.get_source_info());
    output.text("Description:");
    format_text(feature.get_description());
}

void DisplayVisitor::operator()(const Choosable& choosable) {
    output.text(choosable.get_name());
    output.formatted_text("Type: Choosable Feature - %s", choosable.get_type());
    display_source_info(output, choosable.get_source_info());
    output.text("Description:");
    format_text(choosable.get_description());
}

} // namespace dnd
