#include <dnd_config.hpp>

#include "spell_components.hpp"

#include <cassert>
#include <string>
#include <utility>

#include <fmt/format.h>

dnd::SpellComponents::SpellComponents(
    bool verbal, bool somatic, bool material, const std::string& material_components = ""
) noexcept
    : verbal(verbal), somatic(somatic), material(material), material_components(material_components) {}

dnd::SpellComponents::SpellComponents(
    bool verbal, bool somatic, bool material, std::string&& material_components = ""
) noexcept
    : verbal(verbal), somatic(somatic), material(material), material_components(std::move(material_components)) {}

bool dnd::SpellComponents::has_verbal() const noexcept { return verbal; }

bool dnd::SpellComponents::has_somatic() const noexcept { return somatic; }

bool dnd::SpellComponents::has_material() const noexcept { return material; }

const std::string& dnd::SpellComponents::get_material_components() const noexcept { return material_components; }

std::string dnd::SpellComponents::str() const {
    if (!material || material_components.empty()) {
        return short_str();
    }
    return fmt::format("{} ({})", short_str(), material_components);
}

std::string dnd::SpellComponents::short_str() const {
    assert(verbal || somatic || material);
    if (verbal && somatic && material) {
        return "V, S, M";
    } else if (verbal && somatic) {
        return "V, S";
    } else if (verbal && material) {
        return "V, M";
    } else if (somatic && material) {
        return "S, M";
    } else if (verbal) {
        return "V";
    } else if (somatic) {
        return "S";
    } else if (material) {
        return "M";
    }
    return "";
}
