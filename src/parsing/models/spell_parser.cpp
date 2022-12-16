#include "spell_parser.hpp"

#include <memory>
#include <string>

#include <nlohmann/json.hpp>

#include "../../models/spell.hpp"

std::unique_ptr<dnd::Spell> dnd::SpellParser::createSpell(
    const std::string& spell_name, const nlohmann::json& spell_json) {
    // TODO: catch possible exceptions
    const std::string name = spell_name;
    return std::make_unique<Spell>(Spell(name));
}
