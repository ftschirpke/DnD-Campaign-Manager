#ifndef SPELLCASTING_PARSER_HPP_
#define SPELLCASTING_PARSER_HPP_

#include "dnd_config.hpp"

#include <array>
#include <memory>
#include <string>
#include <unordered_map>

#include <nlohmann/json.hpp>

#include "models/spell.hpp"
#include "models/spellcasting/preparation_spellcasting.hpp"
#include "models/spellcasting/spellcasting.hpp"
#include "models/spellcasting/spells_known_spellcasting.hpp"
#include "parsing/subparser.hpp"

namespace dnd {

enum SpellcastingType {
    PREPARATION,
    SPELLS_KNOWN,
};

class SpellcastingParser : public Subparser {
public:
    SpellcastingParser(const std::unordered_map<std::string, const Spell>& spells) noexcept;
    void parseSpellcasting(const nlohmann::json& spellcasting_json);
    std::unique_ptr<Spellcasting> retrieveSpellcasting();
private:
    const std::unordered_map<std::string, const Spell>& spells;
    std::string ability;
    bool ritual_casting;
    std::unordered_map<std::string, const Spell*> spell_list;
    SpellcastingType spellcasting_type;
    PreparationSpellcastingType preparation_spellcasting_type;
    std::array<int, 20> spells_known;
    std::array<int, 20> cantrips_known;
    std::array<std::array<int, 20>, 9> spell_slots;
    void parseSize20Array(const nlohmann::json& json_to_parse, const char* attribute_name, std::array<int, 20>& output);
};

inline SpellcastingParser::SpellcastingParser(const std::unordered_map<std::string, const Spell>& spells) noexcept
    : Subparser(), spells(spells) {}

} // namespace dnd

#endif // SPELLCASTING_PARSER_HPP_
