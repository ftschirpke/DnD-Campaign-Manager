#include "content_controller.hpp"

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#include "models/character.hpp"
#include "models/character_class.hpp"
#include "models/character_race.hpp"
#include "models/spell.hpp"

void dnd::ContentController::printStatus() const {
    std::cout << "=== Spells ===\n";
    std::cout << "spells parsed: " << spells.size() << '\n';
    std::cout << "=== Character Races ===\n";
    std::cout << "character races parsed: " << character_races.size() << '\n';
    std::cout << "character subraces parsed: " << character_subraces.size() << '\n';
    std::cout << "=== Character Classes ===\n";
    std::cout << "character classes parsed: " << character_classes.size() << '\n';
    std::cout << "character subclasses parsed: " << character_subclasses.size() << '\n';
}
