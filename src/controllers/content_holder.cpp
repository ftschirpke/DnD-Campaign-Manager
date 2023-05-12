#include "dnd_config.hpp"

#include "content_holder.hpp"

#include <sstream>
#include <string>
#include <unordered_map>

#include "controllers/content_library.hpp"
#include "models/effect_holder/feature.hpp"
#include "models/feature_holder.hpp"

void add_features_of_feature_holder(
    dnd::ContentLibrary<const dnd::Feature*>& features_library, const dnd::FeatureHolder* feature_holder
) {
    for (const auto& feature : feature_holder->features) {
        features_library.add(feature.name, &feature);
    }
}

void dnd::ContentHolder::finished_parsing() {
    for (const auto& [_, character_class] : character_classes.get_all()) {
        add_features_of_feature_holder(features, &character_class);
    }
    for (const auto& [_, character_subclass] : character_subclasses.get_all()) {
        add_features_of_feature_holder(features, &character_subclass);
    }
    for (const auto& [_, character_race] : character_races.get_all()) {
        add_features_of_feature_holder(features, &character_race);
    }
    for (const auto& [_, character_subrace] : character_subraces.get_all()) {
        add_features_of_feature_holder(features, &character_subrace);
    }

    for (const auto& [group_name, group_choosables] : groups.getAllChoosableGroups()) {
        choosables[group_name] = ContentLibrary<const Choosable*>();
        for (const auto& [choosable_name, choosable] : group_choosables) {
            choosables[group_name].add(choosable_name, &choosable);
        }
    }
}

std::string dnd::ContentHolder::printStatus() const {
    std::stringstream sstr;
    sstr << groups.printStatus();
    sstr << "=== Items ===\n";
    sstr << "items parsed: " << items.size() << '\n';
    sstr << "=== Spells ===\n";
    sstr << "spells parsed: " << spells.size() << '\n';
    sstr << "=== Character Races ===\n";
    sstr << "character races parsed: " << character_races.size() << '\n';
    sstr << "character subraces parsed: " << character_subraces.size() << '\n';
    sstr << "=== Character Classes ===\n";
    sstr << "character classes parsed: " << character_classes.size() << '\n';
    sstr << "character subclasses parsed: " << character_subclasses.size() << '\n';
    sstr << "=== Characters ===\n";
    sstr << "characters parsed: " << characters.size() << '\n';
    return sstr.str();
}
