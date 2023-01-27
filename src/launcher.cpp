#include "dnd_config.hpp"

#include "launcher.hpp"

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

#include <cxxopts.hpp>

#include "controllers/content.hpp"
#include "parsing/content_parser.hpp"
#include "parsing/parsing_exceptions.hpp"

int dnd::launch(int argc, char** argv) {
    DND_MEASURE_FUNCTION();
    const std::filesystem::path cur_path = std::filesystem::current_path();

    cxxopts::Options options(DND_CAMPAIGN_MANAGER_NAME, DND_CAMPAIGN_MANAGER_DESCRIPTION);

    options.add_options()("c,campaign", "Name of campaign directory", cxxopts::value<std::string>())(
        "d,directory", "Content directory", cxxopts::value<std::string>()->default_value((cur_path / "content").c_str())
    )("v,version", "Print version")("h,help", "Print usage");

    cxxopts::ParseResult args;
    try {
        args = options.parse(argc, argv);
    } catch (const cxxopts::OptionParseException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    if (args.count("help")) {
        std::cout << options.help() << '\n';
        return 0;
    }
    if (args.count("version")) {
        std::cout << DND_CAMPAIGN_MANAGER_NAME << " Version " << DND_CAMPAIGN_MANAGER_VERSION_MAJOR << '.'
                  << DND_CAMPAIGN_MANAGER_VERSION_MINOR << '.' << DND_CAMPAIGN_MANAGER_VERSION_PATCH << '\n';
        return 0;
    }
    if (args.count("campaign") != 1) {
        std::cerr << "Error: Please provide exactly one campaign directory.\n";
        return -1;
    }
    if (args.count("directory") > 1) {
        std::cerr << "Error: Please provide only one directory.\n";
        return -1;
    }

    try {
        DND_MEASURE_SCOPE("Main execution scope");
        const std::filesystem::path content_path(args["directory"].as<std::string>());
        const std::string campaign_dir_name = args["campaign"].as<std::string>();
        std::cout << "Content directory:       " << content_path.c_str() << '\n';
        std::cout << "Campaign directory name: " << campaign_dir_name << "\n\n";
        if (campaign_dir_name.size() == 0) {
            throw std::invalid_argument("Campaign directory name cannot be \"\".");
        }
        ContentParser parser;
        Content content = parser.parse(content_path, campaign_dir_name);
        content.printStatus();
    } catch (const parsing_error& e) {
        std::cerr << "Parsing Error: " << e.what() << '\n';
        return -1;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid Argument: " << e.what() << '\n';
        return -1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return -1;
    }

    return 0;
}
