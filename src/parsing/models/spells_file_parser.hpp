#ifndef SPELLS_FILE_PARSER_HPP_
#define SPELLS_FILE_PARSER_HPP_

#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include <nlohmann/json.hpp>

#include "models/spell.hpp"
#include "parsing/content_file_parser.hpp"

namespace dnd {

struct SpellParsingInfo {
    std::string name, casting_time, range, duration, description;
    SpellType type;
    SpellComponents components;
};

class SpellsFileParser : public ContentFileParser {
private:
    std::unordered_map<std::string, const Spell>& results;
    int spells_in_file;
    std::vector<SpellParsingInfo> spell_parsing_info;
    mutable std::vector<bool> valid;
    std::mutex spell_parsing_mutex;
protected:
    void createSpell(std::string_view spell_name, const nlohmann::json& spell_json_ptr);
    SpellType createSpellType(const std::string& spell_type_str) const;
    SpellComponents createSpellComponents(const std::string& spell_components_str) const;
public:
    SpellsFileParser(std::unordered_map<std::string, const Spell>& results);
    void parse() override;
    bool validate() const override;
    void saveResult() override;
};

inline SpellsFileParser::SpellsFileParser(std::unordered_map<std::string, const Spell>& results) : results(results) {}

} // namespace dnd

#endif // SPELLS_FILE_PARSER_HPP_
