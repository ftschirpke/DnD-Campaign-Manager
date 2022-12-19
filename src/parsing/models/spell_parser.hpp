#ifndef SPELL_PARSER_HPP_
#define SPELL_PARSER_HPP_

#include <memory>
#include <string>

#include <nlohmann/json.hpp>

#include "models/spell.hpp"

namespace dnd {

class SpellParser {
public:
    static std::unique_ptr<Spell> createSpell(const std::string& spell_name,
        const nlohmann::json& spell_json);
    static std::unique_ptr<SpellComponents> createSpellComponents(
        const std::string& components_str);
    static std::unique_ptr<SpellType> createSpellType(const std::string& type_str);
};

} // namespace dnd

#endif // SPELL_PARSER_HPP_
