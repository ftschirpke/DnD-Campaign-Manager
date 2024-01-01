#ifndef SPELL_COMPONENTS_DATA_HPP_
#define SPELL_COMPONENTS_DATA_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <string>

#include <core/errors/errors.hpp>

namespace dnd {

class SpellComponentsData {
public:
    std::strong_ordering operator<=>(const SpellComponentsData&) const noexcept = default;

    std::string str;
};

Errors validate_spell_components(const SpellComponentsData& data);

} // namespace dnd

#endif // SPELL_COMPONENTS_DATA_HPP_
