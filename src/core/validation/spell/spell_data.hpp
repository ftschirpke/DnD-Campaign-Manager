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
    SpellData() noexcept;
    std::strong_ordering operator<=>(const SpellData&) const noexcept = default;
    virtual std::unique_ptr<ValidationData> pack() const override;

    SpellComponentsData components_data;
    SpellTypeData type_data;

    std::string casting_time;
    std::string range;
    std::string duration;
    std::set<std::string> classes;
};

} // namespace dnd

#endif // SPELL_DATA_HPP_
