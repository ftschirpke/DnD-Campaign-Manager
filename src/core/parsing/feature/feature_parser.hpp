#ifndef FEATURE_PARSER_HPP_
#define FEATURE_PARSER_HPP_

#include <dnd_config.hpp>

#include <vector>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/parsing/effect_holder/effect_holder_parser.hpp>
#include <core/parsing/parser.hpp>
#include <core/validation/feature/feature_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class FeatureParser : public Parser {
public:
    explicit FeatureParser(const std::filesystem::path& filepath) noexcept;
    /**
     * @brief Parses the data from the json into the data object
     * @param json the json to parse
     * @param data the data object to parse into
     * @return the errors that occured during parsing
     */
    Errors parse(nlohmann::ordered_json&& json, FeatureData& data) const;
private:
    EffectHolderParser effect_holder_parser;
};

} // namespace dnd

#endif // FEATURE_PARSER_HPP_
