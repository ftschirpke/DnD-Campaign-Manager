#include "spell_parser.hpp"

std::unique_ptr<dnd::Spell> dnd::SpellParser::createSpell(const nlohmann::json& spell_json) {
    // TODO: catch possible exceptions
    const std::string name = spell_json.at("name");
    return std::make_unique<Spell>(Spell(name));
}
