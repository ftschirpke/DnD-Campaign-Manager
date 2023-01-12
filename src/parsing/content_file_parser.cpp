#include "dnd_config.hpp"

#include "content_file_parser.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>

bool dnd::ContentFileParser::openJSON(const std::filesystem::directory_entry& file) {
    DND_MEASURE_FUNCTION();
    if (!file.is_regular_file()) {
        std::cerr << "Warning: " << file.path() << " is not a regular file.\n";
        return false;
    }
    filepath = file.path();
    if (filepath.extension().c_str() == ".json") {
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
