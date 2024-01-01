#ifndef SPELL_TYPE_DATA_HPP_
#define SPELL_TYPE_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <string>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>

namespace dnd {

class SpellTypeData {
public:
    std::strong_ordering operator<=>(const SpellTypeData&) const noexcept = default;

    std::string str;
};

Errors validate_spell_type(const SpellTypeData& data);

} // namespace dnd

#endif // SPELL_TYPE_DATA_HPP_
