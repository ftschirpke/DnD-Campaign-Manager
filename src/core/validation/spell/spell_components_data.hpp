#ifndef SPELL_COMPONENTS_DATA_HPP_
#define SPELL_COMPONENTS_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <string>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

class SpellComponentsData : public ValidationSubdata {
public:
    SpellComponentsData(const ValidationData* parent) noexcept;
    std::strong_ordering operator<=>(const SpellComponentsData&) const noexcept = default;

    std::string str;
};

} // namespace dnd

#endif // SPELL_COMPONENTS_DATA_HPP_
