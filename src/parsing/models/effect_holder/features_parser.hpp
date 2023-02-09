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

/**
 * @brief A subparser for parsing features.
 */
class FeaturesParser : public Subparser {
public:
    /**
     * @brief Constructs an FeaturesParser
     * @param groups the already-parsed groups
     */
    FeaturesParser(const Groups& groups) noexcept;
    /**
     * @brief Parse features from a JSON
     * @param features_json the JSON that need s ot be parsed
     * @throws attribute_format_error if features_json has wrong format
     */
    void parseFeatures(const nlohmann::json& features_json);
    /**
     * @brief Returns the parsed features while maintaining ownership
     * @return the parsed features as l-value-reference
     */
    const std::vector<Feature>& getFeatures() const;
    /**
     * @brief Returns and gives up ownership of the parsed features
     * @return the parsed features as r-value-reference
     */
    std::vector<Feature>&& retrieveFeatures();
    /**
     * @brief Configure the parsing type and the file path for the parser and all its subparsers
     * @param conf_type the type of content of the file that is being parsed
     * @param conf_filepath the file that is being parsed
     */
    virtual void configure(ParsingType conf_type, const std::filesystem::path& conf_filepath) noexcept override;
protected:
    /**
     * @brief Parse and create a feature
     * @param feature_name name of the feature
     * @param feature_json the body of the feature as JSON containing the properties of the feature
     * @return the feature with the given name and the properties parsed from the body
     * @throws parsing_error if any error occured while trying to parse the feature_json
     * @throws nlohmann::json::out_of_range if any required attribute does not exist
     * @throws nlohmann::json::type_error if any of the parsed attributes have the wrong type
     */
    Feature createFeature(const std::string& feature_name, const nlohmann::json& feature_json) const;
private:
    // the parsed features
    std::vector<Feature> features;
    // the subparser used for parsing effect holders
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
