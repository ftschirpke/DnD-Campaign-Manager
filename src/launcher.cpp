#include "dnd_config.hpp"

#include "launcher.hpp"

#include <filesystem>
#include <memory>

#include <cxxopts.hpp>

#include "cmd_launcher.hpp"
#include "imgui_launcher.hpp"
#include "output/command_line_output.hpp"
#include "output/output.hpp"

int dnd::launch(int argc, char** argv) {
    DND_MEASURE_FUNCTION();
    std::unique_ptr<Output> output = std::make_unique<CommandLineOutput>(CommandLineOutput());
    const std::filesystem::path cur_path = std::filesystem::current_path();

    cxxopts::Options options(DND_CAMPAIGN_MANAGER_NAME, DND_CAMPAIGN_MANAGER_DESCRIPTION);

    options.add_options()("c,campaign", "Name of campaign directory", cxxopts::value<std::string>())(
        "d,directory", "Content directory",
        cxxopts::value<std::string>()->default_value((cur_path / "content").string())
    )("X,X11app", "Start the app with its X11 graphical user interface"
    )("t,testrun", "App starts and does not wait for input")("v,version", "Print version")("h,help", "Print usage");

    cxxopts::ParseResult args;
    try {
        args = options.parse(argc, argv);
    } catch (const cxxopts::exceptions::parsing& e) {
        output->formatted_error("Error: {}", e.what());
        return 1;
    }

    if (args.count("help")) {
        output->error(options.help());
        return 0;
    }
    if (args.count("version")) {
        output->formatted_error(
            "{} Version {}.{}.{}", DND_CAMPAIGN_MANAGER_NAME, DND_CAMPAIGN_MANAGER_VERSION_MAJOR,
            DND_CAMPAIGN_MANAGER_VERSION_MINOR, DND_CAMPAIGN_MANAGER_VERSION_PATCH
        );
        return 0;
    }

    if (args.count("X11app")) {
        return launchGUI();
    } else {
        return launchCMD(args);
    }
}
