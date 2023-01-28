#ifndef SPELLS_FILE_PARSER_HPP_
#define SPELLS_FILE_PARSER_HPP_

#include "dnd_config.hpp"

#include <memory>
#include <mutex>
#include <regex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <nlohmann/json.hpp>

#include "controllers/groups.hpp"
#include "models/spell.hpp"
#include "parsing/content_file_parser.hpp"

namespace dnd {

struct SpellParsingInfo {
    std::string name, casting_time, range, duration, description;
    std::unordered_set<std::string> classes;
    SpellType type;
    SpellComponents components;
};

class SpellsFileParser : public ContentFileParser {
public:
    SpellsFileParser(std::unordered_map<std::string, const Spell>& spells, Groups& groups) noexcept;
    virtual void parse() override;
    virtual bool validate() const override;
    virtual void saveResult() override;
protected:
    static const std::regex spell_components_regex, spell_type_regex;
    void createSpell(std::string_view spell_name, const nlohmann::json& spell_json_ptr);
    SpellType createSpellType(const std::string& spell_type_str) const;
    SpellComponents createSpellComponents(const std::string& spell_components_str) const;
private:
    std::unordered_map<std::string, const Spell>& spells;
    Groups& groups;
    int spells_in_file;
    std::vector<SpellParsingInfo> spell_parsing_info;
    mutable std::vector<bool> valid;
    std::mutex spell_parsing_mutex;
    virtual void configureSubparsers() override;
};

inline SpellsFileParser::SpellsFileParser(
    std::unordered_map<std::string, const Spell>& spells, Groups& groups
) noexcept
    : ContentFileParser(), spells(spells), groups(groups) {}

inline const std::regex SpellsFileParser::spell_components_regex(
    "(V, S, M (\\((.*)\\))|V, S|V, M (\\((.*)\\))|S, M (\\((.*)\\))|V|S|M (\\((.*)\\)))"
);

inline const std::regex SpellsFileParser::spell_type_regex("((1st|2nd|3rd|[4-9]th)-level "
                                                           "([aA]bjuration|[cC]onjuration|[dD]ivination|[eE]nchantment|"
                                                           "[eE]vocation|[iI]llusion|[nN]ecromancy|[tT]ransmutation)"
                                                           "( \\(ritual\\))?)|("
                                                           "([aA]bjuration|[cC]onjuration|[dD]ivination|[eE]nchantment|"
                                                           "[eE]vocation|[iI]llusion|[nN]ecromancy|[tT]ransmutation)"
                                                           " cantrip)");

inline void SpellsFileParser::configureSubparsers() {} // SpellsFileParser has no subparsers

} // namespace dnd

#endif // SPELLS_FILE_PARSER_HPP_
