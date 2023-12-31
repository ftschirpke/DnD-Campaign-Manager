#ifndef SPELLCASTING_DATA_HPP_
#define SPELLCASTING_DATA_HPP_

#include <dnd_config.hpp>

#include <array>
#include <string>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class ClassData;

class SpellcastingData : public ValidationSubdata {
public:
    SpellcastingData(const ValidationData* parent) noexcept;

    bool is_spellcaster;
    std::string ability;
    bool ritual_casting;
    bool is_spells_known_type;
    std::string preparation_spellcasting_type;
    std::array<int, 20> spells_known;
    std::array<int, 20> cantrips_known;
    std::array<std::array<int, 20>, 9> spell_slots;
};

} // namespace dnd

#endif // SPELLCASTING_DATA_HPP_
