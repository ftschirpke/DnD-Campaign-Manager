#ifndef FEATURE_HOLDER_FILE_PARSER_HPP_
#define FEATURE_HOLDER_FILE_PARSER_HPP_

#include "dnd_config.hpp"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include "controllers/groups.hpp"
#include "models/effect_holder/feature.hpp"
#include "parsing/content_file_parser.hpp"
#include "parsing/models/effect_holder/effect_holder_parser.hpp"

namespace dnd {

class FeatureHolderFileParser : public ContentFileParser {
public:
    FeatureHolderFileParser(const Groups& groups) noexcept;
protected:
    Feature createFeature(const std::string& feature_name, const nlohmann::json& feature_json) const;
    void parseFeatures();
    const std::vector<Feature>& getFeatures() const;
    std::vector<Feature>&& retrieveFeatures();
private:
    std::vector<Feature> features;
    EffectHolderParser effect_holder_parser;
    virtual void configureSubparsers() override;
};

inline FeatureHolderFileParser::FeatureHolderFileParser(const Groups& groups) noexcept
    : ContentFileParser(), effect_holder_parser(groups) {}

inline const std::vector<Feature>& FeatureHolderFileParser::getFeatures() const { return features; }

inline std::vector<Feature>&& FeatureHolderFileParser::retrieveFeatures() { return std::move(features); }

} // namespace dnd

#endif // FEATURE_HOLDER_FILE_PARSER_HPP_
