#ifndef SPECIES_PARSER_HPP_
#define SPECIES_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>

#include <nlohmann/json.hpp>

#include <core/errors/errors.hpp>
#include <core/parsing/effects_provider/feature_parser.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/validation/species/species_data.hpp>

namespace dnd {

class SpeciesParser : public FileParser {
public:
    explicit SpeciesParser(const std::filesystem::path& filepath) noexcept;
    virtual Errors parse() override;
    virtual Errors validate(const Content& content) const override;
    virtual void save_result(Content& content) override;
private:
    FeatureParser feature_parser;
    SpeciesData data;
};

} // namespace dnd

#endif // SPECIES_PARSER_HPP_
