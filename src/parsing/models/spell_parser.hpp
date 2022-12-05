#ifndef SPELL_PARSER_HPP_
#define SPELL_PARSER_HPP_

#include <memory>

#include <nlohmann/json.hpp>

#include "../../models/spell.hpp"

namespace dnd {

class SpellParser {
public:
    static std::unique_ptr<Spell> createSpell(const nlohmann::json& spell_json);
};

} // namespace dnd

#endif // SPELL_PARSER_HPP_
