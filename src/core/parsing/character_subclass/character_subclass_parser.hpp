#ifndef CHARACTER_SUBCLASS_PARSER_HPP_
#define CHARACTER_SUBCLASS_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/parsing/effects_provider/feature_parser.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/validation/character_subclass/character_subclass_data.hpp>

namespace dnd {

class CharacterSubclassParser : public FileParser {
public:
    explicit CharacterSubclassParser(const std::filesystem::path& filepath) noexcept;
    /**
     * @brief Parses the json into subclass data
     * @return the errors that occured while parsing
     */
    virtual Errors parse() override;
    /**
     * @brief Validates the parsed subclass data using the given content
     * @param content the content to validate against
     * @return the errors that occured while validating
     */
    virtual Errors validate(const Content& content) const override;
    /**
     * @brief Saves the parsed subclass data into the given content
     * @param content the content to save the parsed subclass into
     */
    virtual void save_result(Content& content) override;
private:
    FeatureParser feature_parser;
    CharacterSubclassData data;
};

} // namespace dnd

#endif // CHARACTER_SUBCLASS_PARSER_HPP_
