#ifndef CHARACTER_PARSER_HPP_
#define CHARACTER_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/parsing/feature/feature_parser.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/validation/character/character_data.hpp>
#include <core/validation/character/decision/decision_data.hpp>

namespace dnd {

class CharacterParser : public FileParser {
public:
    explicit CharacterParser(const std::filesystem::path& filepath) noexcept;
    /**
     * @brief Parses the json into character data
     * @return the errors that occured while parsing
     */
    virtual Errors parse() override;
    /**
     * @brief Sets the context for the character parser
     * @param content the content to set the context to
     */
    virtual void set_context(const Content& content) override;
    /**
     * @brief Validates the parsed character data using the given content
     * @param content the content to validate against
     * @return the errors that occured while validating
     */
    virtual Errors validate(const Content& content) const override;
    /**
     * @brief Saves the parsed character data into the given content
     * @param content the content to save the parsed character into
     */
    virtual void save_result(Content& content) override;
private:
    Errors parse_decision(nlohmann::ordered_json&& decision_json, DecisionData& decision_data) const;

    FeatureParser feature_parser;
    CharacterData data;
};

} // namespace dnd

#endif // CHARACTER_PARSER_HPP_
