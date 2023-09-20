#ifndef CHARACTER_CLASS_PARSER_HPP_
#define CHARACTER_CLASS_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/parsing/feature/feature_parser.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/validation/character_class/character_class_data.hpp>

namespace dnd {

class CharacterClassParser : public FileParser {
public:
    explicit CharacterClassParser(const std::filesystem::path& filepath) noexcept;
    /**
     * @brief Parses the json into class data
     * @return the errors that occured while parsing
     */
    virtual Errors parse() override;
    /**
     * @brief Validates the parsed class data using the given content
     * @param content the content to validate against
     * @return the errors that occured while validating
     */
    virtual Errors validate(const Content& content) const override;
    /**
     * @brief Saves the parsed class data into the given content
     * @param content the content to save the parsed class into
     */
    virtual void save_result(Content& content) override;
private:
    FeatureParser feature_parser;
    CharacterClassData data;
};

} // namespace dnd

#endif // CHARACTER_CLASS_PARSER_HPP_
