#include <dnd_config.hpp>

#include "content_file_parser.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>

bool dnd::ContentFileParser::open_json() {
    DND_MEASURE_FUNCTION();

    if (filepath.extension().string() != ".json") {
        std::cerr << "Warning: " << filepath << " is not a \".json\" file.\n";
        return false;
    }
    std::ifstream json_file(filepath);
    try {
        DND_MEASURE_SCOPE("JSON serialisation");
        json_file >> json_to_parse;
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Warning: Error occured while parsing " << filepath << ":\n" << e.what() << '\n';
        return false;
    }
    return true;
}
