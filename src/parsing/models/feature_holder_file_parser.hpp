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
    // TODO: this is only public for testing purposes, is there a better solution?
public:
    // private:
    void parseAndAddEffect(const std::string& effect_str, Feature& feature) const;
    void parseAndAddActivation(const std::string& activation_str, Feature& feature) const;
    std::shared_ptr<Feature> createFeature(const std::string& feature_name, const nlohmann::json& feature_json) const;
    // protected:
    virtual std::vector<std::shared_ptr<const Feature>> parseFeatures() const;
};

} // namespace dnd

#endif // FEATURE_HOLDER_FILE_PARSER_HPP_
