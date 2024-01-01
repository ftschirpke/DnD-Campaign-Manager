#ifndef FEATURE_PARSER_HPP_
#define FEATURE_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <vector>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/parsing/effects/effects_parser.hpp>
#include <core/parsing/parser.hpp>
#include <core/validation/effects_provider/feature_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class FeatureParser : public Parser {
public:
    explicit FeatureParser(const std::filesystem::path& filepath) noexcept;
    Errors parse_into(nlohmann::ordered_json&& json, FeatureData& data) const;
    Errors parse_multiple_into(nlohmann::ordered_json&& json, std::vector<FeatureData>& data) const;
private:
    EffectsParser effects_parser;
};

} // namespace dnd

#endif // FEATURE_PARSER_HPP_
