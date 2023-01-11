#ifndef SPELLCASTING_FEATURE_HOLDER_FILE_PARSER_HPP_
#define SPELLCASTING_FEATURE_HOLDER_FILE_PARSER_HPP_

#include <array>
#include <memory>

#include <nlohmann/json.hpp>

#include "models/spellcasting/preparation_spellcasting.hpp"
#include "models/spellcasting/spellcasting.hpp"
#include "models/spellcasting/spells_known_spellcasting.hpp"
#include "parsing/models/feature_holder_file_parser.hpp"

namespace dnd {

enum SpellcastingType {
    PREPARATION,
    SPELLS_KNOWN,
};

class SpellcastingFeatureHolderFileParser : public FeatureHolderFileParser {
public:
    SpellcastingFeatureHolderFileParser() noexcept = default;
protected:
    void parseSize20Array(const nlohmann::json& json_to_parse, const char* attribute_name, std::array<int, 20>& output);
    void parseSpellcasting();
    std::unique_ptr<Spellcasting> retrieveSpellcasting();
private:
    std::string ability;
    bool ritual_casting;
    SpellcastingType spellcasting_type;
    PreparationSpellcastingType preparation_spellcasting_type;
    std::array<int, 20> spells_known;
    std::array<int, 20> cantrips_known;
    std::array<std::array<int, 20>, 9> spell_slots;
};

} // namespace dnd

#endif // SPELLCASTING_FEATURE_HOLDER_FILE_PARSER_HPP_
