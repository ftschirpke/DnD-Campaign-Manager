#ifndef CONTENT_PARSER_HPP_
#define CONTENT_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <string>

#include <core/controllers/content_holder.hpp>
#include <core/errors/errors.hpp>

namespace dnd {

/**
 * @brief A struct containing the result of parsing content
 */
struct ParsingResult {
    ContentHolder content;
    Errors errors;
    std::filesystem::path content_path;
    std::string campaign_directory_name;
};

/**
 * @brief A class for parsing content
 */
class ContentParser {
public:
    /**
     * @brief Parses content from content_path for a certain campaign
     * @param content_path the path to the directory the content to be parsed is located in
     * @param campaign_dir_name the name of the campaign-specific directory
     * @return a struct containing the parsed content, errors and further information
     */
    ParsingResult parse(const std::filesystem::path& content_path, const std::string& campaign_dir_name);
protected:
private:
};

} // namespace dnd

#endif // CONTENT_PARSER_HPP_
