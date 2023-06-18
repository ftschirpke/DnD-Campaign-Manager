#ifndef CHARACTER_SUBCLASS_PARSER_HPP_
#define CHARACTER_SUBCLASS_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/parsing/feature/feature_parser.hpp>
#include <core/parsing/parser.hpp>
#include <core/validation/character_subclass/character_subclass_data.hpp>

namespace dnd {

class CharacterSubclassParser : public Parser {
public:
    explicit CharacterSubclassParser(const std::filesystem::path& filepath) noexcept;
    /**
     * @brief Parses the data from the json into the data object
     * @param json the json to parse
     * @param data the data object to parse into
     * @return the errors that occured during parsing
     */
    Errors parse(nlohmann::ordered_json&& json, CharacterSubclassData& data) const;
private:
    FeatureParser feature_parser;
};

} // namespace dnd

#endif // CHARACTER_SUBCLASS_PARSER_HPP_
