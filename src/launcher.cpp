#include "launcher.hpp"

#include <chrono>
#include <filesystem>
#include <iostream>
#include <stdexcept>

#include <cxxopts.hpp>

#include "dndmanager_config.hpp"

#include "controllers/content.hpp"
#include "parsing/content_parser.hpp"
#include "parsing/parsing_exceptions.hpp"

int dnd::launch(int argc, char** argv) {
    const std::filesystem::path cur_path = std::filesystem::current_path();

    cxxopts::Options options(DnDManager_NAME, DnDManager_DESCRIPTION);

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
        std::cout << DnDManager_NAME << " Version " << DnDManager_VERSION_MAJOR << '.' << DnDManager_VERSION_MINOR
                  << '.' << DnDManager_VERSION_PATCH << '\n';
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
        const std::filesystem::path content_path(args["directory"].as<std::string>());
        const std::string campaign_dir_name = args["campaign"].as<std::string>();
        std::cout << "Content directory:       " << content_path << '\n';
        std::cout << "Campaign directory name: \"" << campaign_dir_name << "\"\n\n";
        if (campaign_dir_name.size() == 0) {
            throw std::invalid_argument("Campaign directory name cannot be \"\".");
        }
        ContentParser parser;
        // TODO: should the runtime measurement and status printing stay?
        std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
        Content content = parser.parse(content_path, campaign_dir_name);
        std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> timespan = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
        std::cout << "Time taken for parsing: " << timespan.count() << " seconds\n";
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
