#ifndef SPELL_COMPONENTS_HPP_
#define SPELL_COMPONENTS_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <string>

#include <core/utils/data_result.hpp>

namespace dnd {

class SpellComponents {
public:
    struct Data;

    static CreateResult<SpellComponents> create(Data&& components_data);

    SpellComponents(bool verbal, bool somatic, bool material, const std::string& material_components = "") noexcept;
    SpellComponents(bool verbal, bool somatic, bool material, std::string&& material_components = "") noexcept;

    bool has_verbal() const noexcept;
    bool has_somatic() const noexcept;
    bool has_material() const noexcept;
    const std::string& get_material_components() const noexcept;

    // returns a string representation of spell components "V, M (a pinch of salt)"
    std::string str() const;
    // returns a short string representation of spell components (without material components) e.g. "V, S, M"
    std::string short_str() const;
private:
    bool verbal;
    bool somatic;
    bool material;
    std::string material_components;
};

struct SpellComponents::Data {
    std::strong_ordering operator<=>(const Data&) const noexcept = default;

    std::string str;
};

} // namespace dnd

#endif // SPELL_COMPONENTS_HPP_
