#include <dnd_config.hpp>

#include <gui_app/gui_launcher.hpp>

int main(int argc, char** argv) {
    DND_START_MEASURING_SESSION("LAUNCH", "debug/gui_app_runtime.json");
    DND_UNUSED(argc);
    DND_UNUSED(argv);
    int rv = dnd::launch();
    DND_END_MEASURING_SESSION();
    return rv;
}
