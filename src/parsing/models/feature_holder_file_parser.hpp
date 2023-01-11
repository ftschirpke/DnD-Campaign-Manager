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
    FeatureHolderFileParser() noexcept = default;
protected:
    Feature createFeature(const std::string& feature_name, const nlohmann::json& feature_json) const;
    virtual void parseFeatures();
    const std::vector<Feature>& getFeatures() const;
    std::vector<Feature>&& retrieveFeatures();
private:
    std::vector<Feature> features;
};

inline const std::vector<Feature>& FeatureHolderFileParser::getFeatures() const { return features; }

inline std::vector<Feature>&& FeatureHolderFileParser::retrieveFeatures() { return std::move(features); }

} // namespace dnd

#endif // FEATURE_HOLDER_FILE_PARSER_HPP_
