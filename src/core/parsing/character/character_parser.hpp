#ifndef CHARACTER_PARSER_HPP_
#define CHARACTER_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/models/character/character.hpp>
#include <core/models/character/decision/decision.hpp>
#include <core/parsing/effects_provider/feature_parser.hpp>
#include <core/parsing/file_parser.hpp>

namespace dnd {

class CharacterParser : public FileParser {
public:
    explicit CharacterParser(const std::filesystem::path& filepath) noexcept;
    virtual Errors parse() override;
    virtual void set_context(const Content& content) override;
    virtual void save_result(Content& content) override;
private:
    Errors parse_decision(nlohmann::ordered_json&& decision_json, Decision::Data& decision_data) const;

    FeatureParser feature_parser;
    Character::Data data;
};

} // namespace dnd

#endif // CHARACTER_PARSER_HPP_
