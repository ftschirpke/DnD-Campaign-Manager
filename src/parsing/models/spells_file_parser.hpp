#ifndef SPELLS_FILE_PARSER_HPP_
#define SPELLS_FILE_PARSER_HPP_

#include <memory>
#include <string>
#include <unordered_map>

#include "models/spell.hpp"
#include "parsing/models/content_file_parser.hpp"

namespace dnd {

class SpellsFileParser : public ContentFileParser {
private:
    std::unordered_map<std::string, std::shared_ptr<const Spell>>& results_map;
    std::string name, casting_time, range, duration, description;
    SpellType type;
    SpellComponents components;
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
