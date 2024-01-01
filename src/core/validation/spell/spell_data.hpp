#ifndef SPELL_DATA_HPP_
#define SPELL_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <memory>
#include <set>
#include <string>

#include <core/errors/errors.hpp>
#include <core/validation/spell/spell_components_data.hpp>
#include <core/validation/spell/spell_type_data.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class SpellData : public ValidationData {
public:
    std::strong_ordering operator<=>(const SpellData&) const noexcept = default;

    SpellComponentsData components_data;
    SpellTypeData type_data;

    std::string casting_time;
    std::string range;
    std::string duration;
    std::set<std::string> classes;
};

Errors validate_spell_nonrecursively(const SpellData& data);
Errors validate_spell_recursively(const SpellData& data);

} // namespace dnd

#endif // SPELL_DATA_HPP_
