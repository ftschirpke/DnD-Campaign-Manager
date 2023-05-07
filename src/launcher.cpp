#include "dnd_config.hpp"

#include "launcher.hpp"

#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include <cxxopts.hpp>
#include <fmt/core.h>

#include "controllers/content.hpp"
#include "output/command_line_output.hpp"
#include "output/output.hpp"
#include "parsing/controllers/content_parser.hpp"
#include "parsing/parsing_exceptions.hpp"

int dnd::launch(int argc, char** argv) {
    DND_MEASURE_FUNCTION();
    std::unique_ptr<Output> output = std::make_unique<CommandLineOutput>(CommandLineOutput());
    const std::filesystem::path cur_path = std::filesystem::current_path();

    cxxopts::Options options(DND_CAMPAIGN_MANAGER_NAME, DND_CAMPAIGN_MANAGER_DESCRIPTION);

    options.add_options()("c,campaign", "Name of campaign directory", cxxopts::value<std::string>())(
        "d,directory", "Content directory",
        cxxopts::value<std::string>()->default_value((cur_path / "content").string())
    )("v,version", "Print version")("h,help", "Print usage");

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
    if (args.count("campaign") != 1) {
        output->error("Error: Please provide exactly one campaign directory.");
        return -1;
    }
    if (args.count("directory") > 1) {
        output->error("Error: Please provide only one directory.\n");
        return -1;
    }

    try {
        DND_MEASURE_SCOPE("Main execution scope");
        const std::filesystem::path content_path(args["directory"].as<std::string>());
        const std::string campaign_dir_name = args["campaign"].as<std::string>();
        output->formatted_text("Content directory:       {}", content_path.string());
        output->formatted_text("Campaign directory name: {}", campaign_dir_name);
        if (campaign_dir_name.empty()) {
            throw std::invalid_argument("Campaign directory name cannot be \"\".");
        }
        ContentParser parser;
        Content content = parser.parse(content_path, campaign_dir_name);
        output->text(content.printStatus());

        DND_MEASURE_SCOPE("Main execution scope without parsing");

        output->text("\n=== CHARACTER INITIALISATION ===\n");
        for (auto& [name, character] : content.characters) {
            output->formatted_text("# {}", name);
            character.determineState();
            output->text("#\n\n");
        }
    } catch (const parsing_error& e) {
        output->formatted_error("Parsing Error: {}", e.what());
        return -1;
    } catch (const std::invalid_argument& e) {
        output->formatted_error("Invalid Argument: {}", e.what());
        return -1;
    } catch (const std::exception& e) {
        output->formatted_error("Error: {}", e.what());
        return -1;
    }

    return 0;
}
