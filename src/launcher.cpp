#include "launcher.hpp"

#include <iostream>
#include <filesystem>

#include <cxxopts/cxxopts.hpp>

#include "dndmanager_config.hpp"

int dnd::launch(int argc, char** argv) {
    const std::filesystem::path cur_path = std::filesystem::current_path();

    cxxopts::Options options(DnDManager_NAME, DnDManager_DESCRIPTION);

    options.add_options()
        ("d,directory", "Content directory", cxxopts::value<std::string>()->default_value(cur_path.c_str()))
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

    return 0;
}