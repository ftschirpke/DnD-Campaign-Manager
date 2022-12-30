#include "content_file_parser.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

#include <nlohmann/json.hpp>

bool dnd::ContentFileParser::openJSON(const std::filesystem::directory_entry& file) {
    if (!file.is_regular_file()) {
        std::cerr << "Warning: " << file.path() << " is not a regular file.\n";
        return false;
    }
    filename = file.path().c_str();
    if (filename.compare(filename.length() - 5, filename.length(), ".json") != 0) {
        std::cerr << "Warning: \"" << filename << "\" is not a \".json\" file.\n";
        return false;
    }
    std::ifstream json_file(filename);
    try {
        json_file >> json_to_parse;
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "Warning: Error occured while parsing \"" << filename << "\":\n" << e.what() << '\n';
        return false;
    }
    return true;
}
