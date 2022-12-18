#include "content_controller.hpp"

#include <iostream>

void dnd::ContentController::printStatus() const {
    std::cout << "=== Spells ===\n";
    std::cout << "spells parsed: " << spells.size() << '\n';
    std::cout << "=== Character Races ===\n";
    std::cout << "character races parsed: " << character_races.size() << '\n';
}
