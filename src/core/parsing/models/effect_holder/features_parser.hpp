#ifndef FEATURES_PARSER_HPP_
#define FEATURES_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include <core/controllers/groups.hpp>
#include <core/models/effect_holder/feature.hpp>
#include <core/parsing/models/effect_holder/effect_holder_parser.hpp>
#include <core/parsing/parsing_types.hpp>
#include <core/parsing/subparser.hpp>

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
    FeaturesParser(ParsingType type, const std::filesystem::path& filepath, const Groups& groups) noexcept;
    /**
     * @brief Parse features from a JSON
     * @param features_json the JSON that need s ot be parsed
     * @throws attribute_format_error if features_json has wrong format
     */
    void parseFeatures(const nlohmann::ordered_json& features_json);
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

inline FeaturesParser::FeaturesParser(
    ParsingType type, const std::filesystem::path& filepath, const Groups& groups
) noexcept
    : Subparser(type, filepath), effect_holder_parser(type, filepath, groups) {}

inline const std::vector<Feature>& FeaturesParser::getFeatures() const { return features; }

inline std::vector<Feature>&& FeaturesParser::retrieveFeatures() { return std::move(features); }

} // namespace dnd

#endif // FEATURES_PARSER_HPP_
