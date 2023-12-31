#ifndef SPELL_TYPE_DATA_HPP_
#define SPELL_TYPE_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <string>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class SpellTypeData : public ValidationSubdata {
public:
    SpellTypeData(const ValidationData* parent) noexcept;
    std::strong_ordering operator<=>(const SpellTypeData&) const noexcept = default;

    std::string str;
};

} // namespace dnd

#endif // SPELL_TYPE_DATA_HPP_
