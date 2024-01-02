#ifndef CHOOSABLE_PARSER_HPP_
#define CHOOSABLE_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <vector>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/models/effects_provider/choosable.hpp>
#include <core/parsing/effects_provider/feature_parser.hpp>
#include <core/parsing/parser.hpp>

namespace dnd {

class ChoosableParser : public Parser {
public:
    explicit ChoosableParser(const std::filesystem::path& filepath) noexcept;
    Errors parse_into(nlohmann::ordered_json&& json, Choosable::Data& data) const;
private:
    FeatureParser feature_parser;
};

} // namespace dnd

#endif // CHOOSABLE_PARSER_HPP_
