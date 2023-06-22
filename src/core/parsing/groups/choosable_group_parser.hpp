#ifndef CHOOSABLE_GROUP_PARSER_HPP_
#define CHOOSABLE_GROUP_PARSER_HPP_

#include <dnd_config.hpp>

#include <filesystem>
#include <unordered_map>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/parsing/feature/choosable_feature_parser.hpp>
#include <core/parsing/file_parser.hpp>
#include <core/validation/feature/choosable_feature_data.hpp>

namespace dnd {

class ChoosableGroupParser : public FileParser {
public:
    explicit ChoosableGroupParser(const std::filesystem::path& filepath) noexcept;
    /**
     * @brief Parses the json into choosable groups data
     * @return the errors that occured while parsing
     */
    virtual Errors parse() override;
    /**
     * @brief Returns whether the parser should continue after an invalid parsing
     * @return "true" because there are multiple choosable features in one file and some may still be valid
     */
    virtual bool continue_after_errors() const noexcept override;
    /**
     * @brief Validates the parsed choosable groups data using the given content
     * @param content the content to validate against
     * @return the errors that occured while validating
     */
    virtual Errors validate(const ContentHolder& content) const override;
    /**
     * @brief Saves the parsed choosable groups data into the given content
     * @param content the content to save the parsed choosable groups into
     */
    virtual void save_result(ContentHolder& content) override;
protected:
    ChoosableFeatureParser choosable_feature_parser;
    std::string group_name;
    std::vector<ChoosableFeatureData> data;
    size_t choosable_features_in_file;
    mutable std::vector<bool> feature_data_valid;
};

} // namespace dnd

#endif // CHOOSABLE_GROUP_PARSER_HPP_
