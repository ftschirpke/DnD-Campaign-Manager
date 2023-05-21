#include "dnd_config.hpp"

#include "cmd_app/cmd_launcher.hpp"

int main(int argc, char** argv) {
    DND_START_MEASURING_SESSION("LAUNCH", "debug/cmd_app_runtime.json");
    int rv = dnd::launch(argc, argv);
    DND_END_MEASURING_SESSION();
    return rv;
}
