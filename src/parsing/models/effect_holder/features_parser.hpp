#ifndef FEATURES_PARSER_HPP_
#define FEATURES_PARSER_HPP_

#include "dnd_config.hpp"

#include <filesystem>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include "controllers/groups.hpp"
#include "models/effect_holder/feature.hpp"
#include "parsing/models/effect_holder/effect_holder_parser.hpp"
#include "parsing/parsing_types.hpp"
#include "parsing/subparser.hpp"

namespace dnd {

class FeaturesParser : public Subparser {
public:
    FeaturesParser(const Groups& groups) noexcept;
    void parseFeatures(const nlohmann::json& features_json);
    const std::vector<Feature>& getFeatures() const;
    std::vector<Feature>&& retrieveFeatures();
    virtual void configure(ParsingType conf_type, const std::filesystem::path& conf_filepath) noexcept override;
protected:
    Feature createFeature(const std::string& feature_name, const nlohmann::json& feature_json) const;
private:
    std::vector<Feature> features;
    EffectHolderParser effect_holder_parser;
};

inline FeaturesParser::FeaturesParser(const Groups& groups) noexcept : Subparser(), effect_holder_parser(groups) {}

inline const std::vector<Feature>& FeaturesParser::getFeatures() const { return features; }

inline std::vector<Feature>&& FeaturesParser::retrieveFeatures() { return std::move(features); }

inline void FeaturesParser::configure(ParsingType conf_type, const std::filesystem::path& conf_filepath) noexcept {
    Subparser::configure(conf_type, conf_filepath);
    effect_holder_parser.configure(conf_type, conf_filepath);
}

} // namespace dnd

#endif // FEATURES_PARSER_HPP_
