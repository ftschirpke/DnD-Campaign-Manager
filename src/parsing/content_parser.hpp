#ifndef CONTENT_PARSER_HPP_
#define CONTENT_PARSER_HPP_

#include <filesystem>
#include <memory>
#include <stdexcept>
#include <string>

#include <nlohmann/json.hpp>

#include "controllers/content_controller.hpp"

namespace dnd {

class parsing_error : public std::invalid_argument {
public:
    parsing_error(const std::string& what);
};

inline parsing_error::parsing_error(const std::string& what) : std::invalid_argument(what) {}

class ContentParser {
private:
    const std::filesystem::path content_path;
    const std::string campaign_dir_name;
    ContentController& controller;
    const std::unique_ptr<const nlohmann::json> openJSON(const std::filesystem::directory_entry& file);
    void parseSpells(const std::filesystem::path& directory);
    void parseCharacters(const std::filesystem::path& directory);
    void parseCharacterClasses(const std::filesystem::path& directory);
    void parseCharacterSubclasses(const std::filesystem::path& directory);
    void parseCharacterRaces(const std::filesystem::path& directory);
    void parseCharacterSubraces(const std::filesystem::path& directory);
    void validateCharacterSubclasses() const;
    void validateCharacterSubraces() const;
public:
    ContentParser(
        const std::filesystem::path& content_path, const std::string& campaign_dir_name, ContentController& controller
    );
    void parseAll();
};

inline ContentParser::ContentParser(
    const std::filesystem::path& content_path, const std::string& campaign_dir_name, ContentController& controller
)
    : content_path(content_path), campaign_dir_name(campaign_dir_name), controller(controller) {}

} // namespace dnd

#endif // CONTENT_PARSER_HPP_
