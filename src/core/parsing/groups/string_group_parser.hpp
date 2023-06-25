#ifndef STRING_GROUP_PARSER_HPP_
#define STRING_GROUP_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <set>
#include <unordered_map>

#include <core/errors/errors.hpp>
#include <core/parsing/file_parser.hpp>

namespace dnd {

class StringGroupParser : public FileParser {
public:
    explicit StringGroupParser(const std::filesystem::path& file_path);
    /**
     * @brief Parses the json into string groups data
     * @return the errors that occured while parsing
     */
    virtual Errors parse() override;
    /**
     * @brief Validates the parsed string groups data using the given content
     * @param content the content to validate against
     * @return the errors that occured while validating
     */
    virtual Errors validate(const Content& content) const override;
    /**
     * @brief Saves the parsed string groups data into the given content
     * @param content the content to save the parsed string groups into
     */
    virtual void save_result(Content& content) override;
private:
    Errors parse_subgroups(nlohmann::ordered_json& json, const std::string& parent);

    std::unordered_map<std::string, std::set<std::string>> members;
    std::unordered_map<std::string, std::set<std::string>> subgroups;
};

} // namespace dnd

#endif // STRING_GROUP_PARSER_HPP_
