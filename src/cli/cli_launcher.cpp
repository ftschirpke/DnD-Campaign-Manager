#include <dnd_config.hpp>

#include "cli_launcher.hpp"

#include <exception>
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>

#include <cxxopts.hpp>

#include <cli/cli_app.hpp>
#include <cli/output/command_line_output.hpp>
#include <core/content.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/models/effects_provider/feature.hpp>
#include <core/models/item/item.hpp>
#include <core/models/spell/spell.hpp>
#include <core/parsing/content_parsing.hpp>
#include <core/utils/string_manipulation.hpp>

namespace dnd {

int launch(int argc, char** argv) {
    DND_MEASURE_FUNCTION();
    CommandLineOutput output;
    const std::filesystem::path cur_path = std::filesystem::current_path();

    cxxopts::Options options(DND_CAMPAIGN_MANAGER_NAME, DND_CAMPAIGN_MANAGER_DESCRIPTION);

    options.add_options()("c,campaign", "Name of campaign directory",
        cxxopts::value<std::string>()->default_value("")
    )(
        "d,directory", "Content directory",
        cxxopts::value<std::string>()->default_value((cur_path / "content").string())
    )("t,testrun", "App starts and does not wait for input")("v,version", "Print version")("h,help", "Print usage");

    cxxopts::ParseResult args;
    try {
        args = options.parse(argc, argv);
    } catch (const cxxopts::exceptions::parsing& e) {
        output.formatted_error("Error: {}", e.what());
        return 1;
    }

    if (args.count("help")) {
        output.error(options.help());
        return 0;
    }
    if (args.count("version")) {
        output.formatted_error(
            "{} Version {}.{}.{}", DND_CAMPAIGN_MANAGER_NAME, DND_CAMPAIGN_MANAGER_VERSION_MAJOR,
            DND_CAMPAIGN_MANAGER_VERSION_MINOR, DND_CAMPAIGN_MANAGER_VERSION_PATCH
        );
        return 0;
    }

    bool testrun = args.count("testrun") > 0;

    try {
        DND_MEASURE_SCOPE("Main execution scope");
        const std::filesystem::path content_path(args["directory"].as<std::string>());
        const std::string campaign_dir_name = args["campaign"].as<std::string>();

        CliApp app;
        app.initialize(content_path, campaign_dir_name, testrun);

        {
            DND_MEASURE_SCOPE("Main execution scope without parsing");

            if (testrun) {
                return 0;
            }

            app.run();
        }
    } catch (const std::exception& e) {
        output.formatted_error("Error: {}", e.what());
        return -1;
    }

    return 0;
}

} // namespace dnd
