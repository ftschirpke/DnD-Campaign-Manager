#ifndef CONTENT_PARSER_HPP_
#define CONTENT_PARSER_HPP_

#include <filesystem>

#include <nlohmann/json.hpp>

#include "../models/content_controller.hpp"

namespace dnd {

class ContentParser {
private:
    const std::filesystem::path content_path;
    ContentController& controller;
    const std::unique_ptr<const nlohmann::json> openJSON(const std::filesystem::directory_entry& file);
    void parseSpells(const std::filesystem::path& directory);
public:
    ContentParser(const std::filesystem::path& content_path, ContentController& controller) :
        content_path(content_path), controller(controller) {}
    void parseAll();
};

} // namespace dnd

#endif // CONTENT_PARSER_HPP_
