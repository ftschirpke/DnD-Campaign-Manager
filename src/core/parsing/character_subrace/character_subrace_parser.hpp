#ifndef CHARACTER_SUBRACE_PARSER_HPP_
#define CHARACTER_SUBRACE_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/parsing/effects_provider/feature_parser.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/validation/character_subrace/character_subrace_data.hpp>

namespace dnd {

class CharacterSubraceParser : public FileParser {
public:
    explicit CharacterSubraceParser(const std::filesystem::path& filepath) noexcept;
    /**
     * @brief Parses the json into subrace data
     * @return the errors that occured while parsing
     */
    virtual Errors parse() override;
    /**
     * @brief Validates the parsed subrace data using the given content
     * @param content the content to validate against
     * @return the errors that occured while validating
     */
    virtual Errors validate(const Content& content) const override;
    /**
     * @brief Saves the parsed subrace data into the given content
     * @param content the content to save the parsed subrace into
     */
    virtual void save_result(Content& content) override;
private:
    FeatureParser feature_parser;
    CharacterSubraceData data;
};

} // namespace dnd

#endif // CHARACTER_SUBRACE_PARSER_HPP_
