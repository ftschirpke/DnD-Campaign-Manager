#include "dnd_config.hpp"

#include "content.hpp"

#include <iostream>
#include <string>
#include <unordered_map>

void dnd::Content::printStatus() const {
    groups.printStatus();
    std::cout << "=== Spells ===\n";
    std::cout << "spells parsed: " << spells.size() << '\n';
    std::cout << "=== Character Races ===\n";
    std::cout << "character races parsed: " << character_races.size() << '\n';
    std::cout << "character subraces parsed: " << character_subraces.size() << '\n';
    std::cout << "=== Character Classes ===\n";
    std::cout << "character classes parsed: " << character_classes.size() << '\n';
    std::cout << "character subclasses parsed: " << character_subclasses.size() << '\n';
    std::cout << "=== Characters ===\n";
    std::cout << "characters parsed: " << characters.size() << '\n';
}
