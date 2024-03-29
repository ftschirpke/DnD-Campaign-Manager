#include <dnd_config.hpp>

#include <filesystem>

#include <gui/gui_launcher.hpp>

int main(int argc, char** argv) {
    DND_START_MEASURING_SESSION(
        "LAUNCH", (std::filesystem::path(DND_BENCH_DIRECTORY) / "gui_app_runtime.json").string()
    );
    DND_UNUSED(argc);
    DND_UNUSED(argv);
    int rv = dnd::launch();
    DND_END_MEASURING_SESSION();
    return rv;
}
