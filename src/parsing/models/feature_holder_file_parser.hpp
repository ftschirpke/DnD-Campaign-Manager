#ifndef FEATURE_HOLDER_FILE_PARSER_HPP_
#define FEATURE_HOLDER_FILE_PARSER_HPP_

#include <memory>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "models/effects_holder/activation.hpp"
#include "models/effects_holder/feature.hpp"
#include "parsing/models/effects_holder_file_parser.hpp"

namespace dnd {

class FeatureHolderFileParser : public EffectsHolderFileParser {
public:
    FeatureHolderFileParser() = default;
protected:
    std::vector<Feature> features;
    Feature createFeature(const std::string& feature_name, const nlohmann::json& feature_json) const;
    virtual void parseFeatures();
};

} // namespace dnd

#endif // FEATURE_HOLDER_FILE_PARSER_HPP_
