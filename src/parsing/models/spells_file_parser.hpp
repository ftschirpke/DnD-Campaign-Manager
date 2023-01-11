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
public:
    SpellsFileParser(std::unordered_map<std::string, const Spell>& results) noexcept;
    virtual void parse() override;
    virtual bool validate() const override;
    virtual void saveResult() override;
protected:
    static const std::string spell_type_regex_str, spell_components_regex_str;
    void createSpell(std::string_view spell_name, const nlohmann::json& spell_json_ptr);
    SpellType createSpellType(const std::string& spell_type_str) const;
    SpellComponents createSpellComponents(const std::string& spell_components_str) const;
private:
    std::unordered_map<std::string, const Spell>& results;
    int spells_in_file;
    std::vector<SpellParsingInfo> spell_parsing_info;
    mutable std::vector<bool> valid;
    std::mutex spell_parsing_mutex;
};

inline SpellsFileParser::SpellsFileParser(std::unordered_map<std::string, const Spell>& results) noexcept
    : results(results) {}

inline const std::string SpellsFileParser::spell_type_regex_str =
    "((1st|2nd|3rd|[4-9]th)-level "
    "([aA]bjuration|[cC]onjuration|[dD]ivination|[eE]nchantment|"
    "[eE]vocation|[iI]llusion|[nN]ecromancy|[tT]ransmutation)"
    "( \\(ritual\\))?)|("
    "([aA]bjuration|[cC]onjuration|[dD]ivination|[eE]nchantment|"
    "[eE]vocation|[iI]llusion|[nN]ecromancy|[tT]ransmutation)"
    " cantrip)";

inline const std::string SpellsFileParser::spell_components_regex_str =
    "(V, S, M (\\((.*)\\))|V, S|V, M (\\((.*)\\))|S, M (\\((.*)\\))|V|S|M (\\((.*)\\)))";

} // namespace dnd

#endif // SPELLS_FILE_PARSER_HPP_
