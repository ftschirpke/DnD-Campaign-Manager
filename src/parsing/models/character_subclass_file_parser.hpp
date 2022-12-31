#ifndef CHARACTER_SUBCLASS_FILE_PARSER_HPP_
#define CHARACTER_SUBCLASS_FILE_PARSER_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "models/character_class.hpp"
#include "models/features/feature.hpp"
#include "parsing/models/feature_holder_file_parser.hpp"

namespace dnd {

class CharacterSubclassFileParser : public FeatureHolderFileParser {
private:
    std::unordered_map<std::string, std::shared_ptr<const CharacterSubclass>>& results;
    const std::unordered_map<std::string, std::shared_ptr<const CharacterClass>>& classes;
    std::string character_subclass_name, class_name;
    std::vector<std::shared_ptr<const Feature>> features;
public:
    CharacterSubclassFileParser(
        std::unordered_map<std::string, std::shared_ptr<const CharacterSubclass>>& results,
        const std::unordered_map<std::string, std::shared_ptr<const CharacterClass>>& classes
    );
    void parse() override;
    bool validate() const override;
    void saveResult() override;
    void reset() override;
};

inline CharacterSubclassFileParser::CharacterSubclassFileParser(
    std::unordered_map<std::string, std::shared_ptr<const CharacterSubclass>>& results,
    const std::unordered_map<std::string, std::shared_ptr<const CharacterClass>>& classes
)
    : results(results), classes(classes) {}

} // namespace dnd

#endif // CHARACTER_SUBCLASS_FILE_PARSER_HPP_
