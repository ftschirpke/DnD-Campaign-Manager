#include "dnd_config.hpp"

#include "groups.hpp"

#include <iostream>
#include <unordered_map>

void dnd::Groups::printStatus() const {
    std::cout << "=== Groups ===\n";
    std::cout << "string-based groups parsed: " << data.size() << '\n';
    std::cout << "choosable-based groups parsed: " << choosables.size() << '\n';
}
