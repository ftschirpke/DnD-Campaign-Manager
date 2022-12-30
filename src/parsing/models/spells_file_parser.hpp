#ifndef SPELLS_FILE_PARSER_HPP_
#define SPELLS_FILE_PARSER_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "models/spell.hpp"
#include "parsing/models/content_file_parser.hpp"

namespace dnd {

class SpellsFileParser : public ContentFileParser {
private:
    std::unordered_map<std::string, std::shared_ptr<const Spell>>& results_map;
    int spells_in_file;
    std::vector<std::string> names, casting_times, ranges, durations, descriptions;
    std::vector<SpellType> types;
    std::vector<SpellComponents> components;
    mutable std::vector<bool> valid;
    SpellType createSpellType(const std::string& spell_type_str);
    SpellComponents createSpellComponents(const std::string& spell_components_str);
public:
    SpellsFileParser(std::unordered_map<std::string, std::shared_ptr<const Spell>>& results_map);
    void parse() override;
    bool validate() const override;
    void saveResult() override;
    void reset() override;
};

inline SpellsFileParser::SpellsFileParser(std::unordered_map<std::string, std::shared_ptr<const Spell>>& results_map)
    : results_map(results_map) {}

} // namespace dnd

#endif // SPELLS_FILE_PARSER_HPP_
