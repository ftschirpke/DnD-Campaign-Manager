#ifndef FILE_PARSER_HPP_
#define FILE_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>

#include <nlohmann/json.hpp>

#include <core/parsing/parser.hpp>

namespace dnd {

class ContentHolder;

class FileParser : public Parser {
public:
    explicit FileParser(const std::filesystem::path& filepath) noexcept;
    /**
     * @brief Opens the json file and saves it in the json variable
     * @return the errors that occured while opening the file
     */
    Errors open_json() final;
    /**
     * @brief Parses the json into content piece data
     * @return the errors that occured while parsing
     */
    virtual Errors parse() = 0;
    /**
     * @brief Validates the parsed content piece data using the given content
     * @param content the content to validate against
     * @return the errors that occured while validating
     */
    virtual Errors validate(const ContentHolder& content) const = 0;
    /**
     * @brief Saves the parsed content piece data into the given content
     * @param content the content to save the parsed data into
     */
    virtual void save_result(ContentHolder& content) = 0;
protected:
    nlohmann::ordered_json json;
};

} // namespace dnd

#endif // FILE_PARSER_HPP_
