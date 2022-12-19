#ifndef CONTENT_PARSER_HPP_
#define CONTENT_PARSER_HPP_

#include <filesystem>
#include <stdexcept>

#include <nlohmann/json.hpp>

#include "controllers/content_controller.hpp"

namespace dnd {

class parsing_error : public std::invalid_argument {
public:
    parsing_error(const std::string& what) : std::invalid_argument(what) {}
};

class ContentParser {
private:
    const std::filesystem::path content_path;
    ContentController& controller;
    const std::unique_ptr<const nlohmann::json> openJSON(const std::filesystem::directory_entry& file);
    void parseSpells(const std::filesystem::path& directory);
    void parseCharacterClasses(const std::filesystem::path& directory);
    void parseCharacterSubclasses(const std::filesystem::path& directory);
    void parseCharacterRaces(const std::filesystem::path& directory);
    void parseCharacterSubraces(const std::filesystem::path& directory);
    void validateCharacterSubclasses() const;
    void validateCharacterSubraces() const;
public:
    ContentParser(const std::filesystem::path& content_path, ContentController& controller) :
        content_path(content_path), controller(controller) {}
    void parseAll();
};

} // namespace dnd

#endif // CONTENT_PARSER_HPP_
