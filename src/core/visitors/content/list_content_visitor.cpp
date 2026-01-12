#include <dnd_config.hpp>

#include "list_content_visitor.hpp"

#include <string>
#include <vector>

#include <fmt/format.h>

#include <core/models/character/character.hpp>
#include <core/models/class/class.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/species/species.hpp>
#include <core/models/spell/spell.hpp>
#include <core/models/subclass/subclass.hpp>
#include <core/models/subspecies/subspecies.hpp>
#include <core/visitors/content/content_visitor.hpp>

namespace dnd {

ListContentVisitor::ListContentVisitor(const Content& content) noexcept : content(content) {}

void ListContentVisitor::reserve(size_t size) { string_list.reserve(size); }

const std::vector<std::string>& ListContentVisitor::get_list() const { return string_list; }

std::vector<std::string> ListContentVisitor::get_list() { return std::move(string_list); }

void ListContentVisitor::clear_list() { string_list.clear(); }

void ListContentVisitor::visit(const Character& character) {
    const Class& cls = content.get_class(character.get_feature_providers().get_class_id());
    const Species& species = content.get_species(character.get_feature_providers().get_species_id());
    string_list.push_back(
        fmt::format(
            "{} ({}) [CHARACTER] : Level {} {} {}##{}", character.get_name(), character.get_source_info().name,
            character.get_progression().get_level(), cls.get_name(), species.get_name(), character.get_key()
        )
    );
}

void ListContentVisitor::visit(const Class& cls) {
    string_list.push_back(
        fmt::format("{} ({}) [CLASS]##{}", cls.get_name(), cls.get_source_info().name, cls.get_key())
    );
}

void ListContentVisitor::visit(const Subclass& subclass) {
    Id class_id = subclass.get_class_id();
    const Class& cls = content.get_class(class_id);
    string_list.push_back(
        fmt::format(
            "{} ({}) [{} SUBCLASS]##{}", subclass.get_name(), subclass.get_source_info().name, cls.get_name(),
            subclass.get_key()
        )
    );
}
void ListContentVisitor::visit(const Species& species) {
    string_list.push_back(
        fmt::format("{} ({}) [SPECIES]##{}", species.get_name(), species.get_source_info().name, species.get_key())
    );
}

void ListContentVisitor::visit(const Subspecies& subspecies) {
    const Species& species = subspecies.get_species();
    string_list.push_back(
        fmt::format(
            "{} ({}) [{} SUBSPECIES]##{}", subspecies.get_name(), subspecies.get_source_info().name, species.get_name(),
            subspecies.get_key()
        )
    );
}

void ListContentVisitor::visit(const Item& item) {
    if (item.requires_attunement()) {
        string_list.push_back(
            fmt::format(
                "{} ({}) [ITEM] requires attunement##{}", item.get_name(), item.get_source_info().name, item.get_key()
            )
        );
    } else {
        string_list.push_back(
            fmt::format(
                "{} ({}) [ITEM] no attunement##{}", item.get_name(), item.get_source_info().name, item.get_key()
            )
        );
    }
}

void ListContentVisitor::visit(const Spell& spell) {
    string_list.push_back(
        fmt::format(
            "{} ({}) [SPELL] : {}##{}", spell.get_name(), spell.get_source_info().name, spell.get_type().short_str(),
            spell.get_key()
        )
    );
}

void ListContentVisitor::visit(const Feature& feature) {
    string_list.push_back(
        fmt::format(
            "{} ({}) [FEATURE] : {}##{}", feature.get_name(), feature.get_source_info().name,
            feature.get_source_info().name, feature.get_key()
        )
    );
}

void ListContentVisitor::visit(const ClassFeature& class_feature) {
    string_list.push_back(
        fmt::format(
            "{} ({}) [CLASS FEATURE] : {}##{} from {}##{}", class_feature.get_name(),
            class_feature.get_source_info().name, class_feature.get_source_info().name, class_feature.get_key(),
            class_feature.get_class_name(), class_feature.get_class_source_name()
        )
    );
}

void ListContentVisitor::visit(const SubclassFeature& subclass_feature) {
    string_list.push_back(
        fmt::format(
            "{} ({}) [SUBCLASS FEATURE] : {}##{} from {}##{}", subclass_feature.get_name(),
            subclass_feature.get_source_info().name, subclass_feature.get_source_info().name,
            subclass_feature.get_key(), subclass_feature.get_subclass_short_name(),
            subclass_feature.get_subclass_source_name()
        )
    );
}


void ListContentVisitor::visit(const Choosable& choosable) {
    string_list.push_back(
        fmt::format(
            "{} ({}) [CHOOSABLE] : {}##{}", choosable.get_name(), choosable.get_source_info().name,
            choosable.get_source_info().name, choosable.get_key()
        )
    );
}

} // namespace dnd
