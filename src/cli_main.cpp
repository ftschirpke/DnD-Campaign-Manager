#include <dnd_config.hpp>

#include <cli_app/cli_launcher.hpp>

int main(int argc, char** argv) {
    DND_START_MEASURING_SESSION("LAUNCH", "debug/cli_app_runtime.json");
    int rv = dnd::launch(argc, argv);
    DND_END_MEASURING_SESSION();
    return rv;
}
