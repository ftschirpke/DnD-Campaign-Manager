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
    virtual Errors parse() override;
    virtual Errors validate(const Content& content) const override;
    virtual void save_result(Content& content) override;
private:
    FeatureParser feature_parser;
    CharacterSubraceData data;
};

} // namespace dnd

#endif // CHARACTER_SUBRACE_PARSER_HPP_
