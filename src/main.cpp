#include "dnd_config.hpp"

#include <iostream>
#include <type_traits>

#include "launcher.hpp"

int main(int argc, char** argv) {
    std::cout << std::boolalpha << std::is_trivially_destructible_v<dnd::Measurer> << '\n';
    DND_START_MEASURING_SESSION("LAUNCH", "debug/runtime_measurement_results.json");
    { DND_MEASURE_SCOPE("FIRST"); }
    int rv = dnd::launch(argc, argv);
    DND_END_MEASURING_SESSION();
    return rv;
}
