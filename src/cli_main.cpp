#include <dnd_config.hpp>

#include <filesystem>

#include <cli_app/cli_launcher.hpp>

int main(int argc, char** argv) {
    DND_START_MEASURING_SESSION("LAUNCH", std::filesystem::path(DND_DEBUG_DIRECTORY) / "cli_app_runtime.json");
    int rv = dnd::launch(argc, argv);
    DND_END_MEASURING_SESSION();
    return rv;
}
