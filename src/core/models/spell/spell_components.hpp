#ifndef SPELL_COMPONENTS_HPP_
#define SPELL_COMPONENTS_HPP_

#include <dnd_config.hpp>

#include <string>

namespace dnd {

class SpellComponents {
public:
    SpellComponents(bool verbal, bool somatic, bool material, const std::string& material_components = "") noexcept;
    SpellComponents(bool verbal, bool somatic, bool material, std::string&& material_components = "") noexcept;

    bool has_verbal() const noexcept;
    bool has_somatic() const noexcept;
    bool has_material() const noexcept;
    const std::string& get_material_components() const noexcept;

    /**
     * @brief Creates a string representation of the spell components
     * example: "V, M (a pinch of salt)"
     * @return the string representation of the spell components
     */
    std::string str() const;
    /**
     * @brief Creates a short string representation of the spell components (without the material components)
     * example: "V, S, M"
     * @return the short string representation of the spell components
     */
    std::string short_str() const;
private:
    bool verbal;
    bool somatic;
    bool material;
    std::string material_components;
};

} // namespace dnd

#endif // SPELL_COMPONENTS_HPP_
