#ifndef CLASS_FEATURE_PARSER_HPP_
#define CLASS_FEATURE_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <vector>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/models/effects_provider/class_feature.hpp>
#include <core/parsing/effects/effects_parser.hpp>
#include <core/parsing/parser.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class ClassFeatureParser : public Parser {
public:
    explicit ClassFeatureParser(const std::filesystem::path& filepath) noexcept;
    Errors parse_into(nlohmann::ordered_json&& json, ClassFeature::Data& data) const;
    Errors parse_multiple_into(nlohmann::ordered_json&& json, std::vector<ClassFeature::Data>& data) const;
private:
    EffectsParser effects_parser;
};

} // namespace dnd

#endif // CLASS_FEATURE_PARSER_HPP_
