#ifndef FEATURE_HOLDER_FILE_PARSER_HPP_
#define FEATURE_HOLDER_FILE_PARSER_HPP_

#include <memory>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "models/features/activation.hpp"
#include "models/features/feature.hpp"
#include "parsing/content_file_parser.hpp"

namespace dnd {

class FeatureHolderFileParser : public ContentFileParser {
protected:
    std::vector<std::shared_ptr<const Feature>> features;
    void parseAndAddEffect(const std::string& effect_str, Feature& feature) const;
    void parseAndAddActivation(const std::string& activation_str, Feature& feature) const;
    std::shared_ptr<Feature> createFeature(const std::string& feature_name, const nlohmann::json& feature_json) const;
    virtual void parseFeatures();
public:
    virtual void reset() override;
};

} // namespace dnd

#endif // FEATURE_HOLDER_FILE_PARSER_HPP_
