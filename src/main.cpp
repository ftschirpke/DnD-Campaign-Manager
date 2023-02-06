#include "dnd_config.hpp"

#include "launcher.hpp"

int main(int argc, char** argv) {
    DND_START_MEASURING_SESSION("LAUNCH", "debug/runtime_measurement_results.json");
    { DND_MEASURE_SCOPE("FIRST"); }
    int rv = dnd::launch(argc, argv);
    DND_END_MEASURING_SESSION();
    return rv;
}
