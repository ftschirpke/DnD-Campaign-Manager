#ifndef SUBSPECIES_PARSER_HPP_
#define SUBSPECIES_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/models/subspecies/subspecies.hpp>
#include <core/parsing/effects_provider/feature_parser.hpp>
#include <core/parsing/file_parser.hpp>

namespace dnd {

class SubspeciesParser : public FileParser {
public:
    explicit SubspeciesParser(const std::filesystem::path& filepath) noexcept;
    virtual Errors parse() override;
    virtual void save_result(Content& content) override;
private:
    FeatureParser feature_parser;
    Subspecies::Data data;
};

} // namespace dnd

#endif // SUBSPECIES_PARSER_HPP_
