#include "launcher.hpp"

#include <iostream>
#include <filesystem>
#include <stdexcept>

#include <cxxopts/cxxopts.hpp>

#include "dndmanager_config.hpp"
#include "models/content_controller.hpp"
#include "parsing/content_parser.hpp"

int dnd::launch(int argc, char** argv) {
    const std::filesystem::path cur_path = std::filesystem::current_path();

    cxxopts::Options options(DnDManager_NAME, DnDManager_DESCRIPTION);

    options.add_options()
        ("d,directory", "Content directory", cxxopts::value<std::string>()
            ->default_value((cur_path/"content").c_str())
        )
        ("v,version", "Print version")
        ("h,help", "Print usage")
    ;

    cxxopts::ParseResult args;
    try {
        args = options.parse(argc, argv);
    } catch (const cxxopts::OptionParseException& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    if (args.count("help")) {
        std::cout << options.help() << "\n";
        return 0;
    }
    if (args.count("version")) {
        std::cout << DnDManager_NAME << " Version "
            << DnDManager_VERSION_MAJOR << "."
            << DnDManager_VERSION_MINOR << "."
            << DnDManager_VERSION_PATCH << "\n";
        return 0;
    }
    if (args.count("directory") > 1) {
        std::cerr << "Error: Please provide only one directory." << '\n';
        return -1;
    }

    try {
        const std::filesystem::path content_path(args["directory"].as<std::string>());
        ContentController controller;
        ContentParser parser(content_path, controller);
        parser.parseAll();

        // just for the moment: (TODO: remove later)
        std::cout << "=== Spells ===\n";
        for (const auto& spell : controller.spells) {
            std::cout << spell.first << '\n';
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return -1;
    }

    return 0;
}