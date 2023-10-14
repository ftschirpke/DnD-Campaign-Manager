#ifndef CLASS_FEATURE_PARSER_HPP_
#define CLASS_FEATURE_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <vector>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/parsing/effects/effects_parser.hpp>
#include <core/parsing/parser.hpp>
#include <core/validation/effects_provider/class_feature_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class ClassFeatureParser : public Parser {
public:
    explicit ClassFeatureParser(const std::filesystem::path& filepath) noexcept;
    /**
     * @brief Parses the data from the json into the data object
     * @param json the json to parse
     * @param data the data object to parse into
     * @return the errors that occured during parsing
     */
    Errors parse(nlohmann::ordered_json&& json, ClassFeatureData& data) const;
    /**
     * @brief Parses the data from the json into the data objects
     * @param json the json to parse
     * @param data the data objects to parse into
     * @return the errors that occured during parsing
     */
    Errors parse_multiple(
        nlohmann::ordered_json&& json, std::vector<ClassFeatureData>& data, const ValidationData* parent
    ) const;
private:
    EffectsParser effects_parser;
};

} // namespace dnd

#endif // CLASS_FEATURE_PARSER_HPP_
