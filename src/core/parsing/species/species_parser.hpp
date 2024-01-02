#ifndef SPECIES_PARSER_HPP_
#define SPECIES_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/models/species/species.hpp>
#include <core/parsing/effects_provider/feature_parser.hpp>
#include <core/parsing/file_parser.hpp>

namespace dnd {

class SpeciesParser : public FileParser {
public:
    explicit SpeciesParser(const std::filesystem::path& filepath) noexcept;
    virtual Errors parse() override;
    virtual void save_result(Content& content) override;
private:
    FeatureParser feature_parser;
    Species::Data data;
};

} // namespace dnd

#endif // SPECIES_PARSER_HPP_
