#include <dnd_config.hpp>

#include "content_holder.hpp"

#include <sstream>
#include <string>
#include <unordered_map>

#include <core/controllers/content_library.hpp>

bool dnd::ContentHolder::empty() const {
    return characters.empty() && character_classes.empty() && character_subclasses.empty() && character_races.empty()
           && character_subraces.empty() && items.empty() && spells.empty();
}

std::string dnd::ContentHolder::status() const {
    std::stringstream sstr;
    sstr << groups.status();
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
