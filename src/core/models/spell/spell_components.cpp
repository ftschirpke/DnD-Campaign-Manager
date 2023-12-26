#include <dnd_config.hpp>

#include "spell_components.hpp"

#include <cassert>
#include <string>
#include <utility>

#include <fmt/format.h>

#include <core/errors/errors.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/validation/spell/spell_components_data.hpp>

namespace dnd {

SpellComponents SpellComponents::create(SpellComponentsData&& components_data) {
    if (!components_data.validate().ok()) {
        throw invalid_data("Cannot create SpellComponents from invalid data.");
    }
    bool verbal, somatic, material;
    std::string materials_needed;

    size_t parentheses_idx = components_data.str.find(" (");
    std::string first_part = (parentheses_idx == std::string::npos) ? components_data.str
                                                                    : components_data.str.substr(0, parentheses_idx);
    if (first_part.size() == 7) {
        verbal = true;
        somatic = true;
        material = true;
    } else if (first_part.size() == 4) {
        verbal = first_part[0] == 'V';
        somatic = first_part[0] == 'S' || first_part[3] == 'S';
        material = first_part[3] == 'M';
    } else if (first_part.size() == 1) {
        verbal = first_part == "V";
        somatic = first_part == "S";
        material = first_part == "M";
    } else {
        // this should never happen, if the validation is correct
        assert(false);
        return SpellComponents(false, false, false, "");
    }
    if (material) {
        materials_needed = components_data.str.substr(
            parentheses_idx + 2, components_data.str.size() - parentheses_idx - 3
        );
    }
    return SpellComponents(verbal, somatic, material, std::move(materials_needed));
}

SpellComponents::SpellComponents(
    bool verbal, bool somatic, bool material, const std::string& material_components
) noexcept
    : verbal(verbal), somatic(somatic), material(material), material_components(material_components) {}

SpellComponents::SpellComponents(
    bool verbal, bool somatic, bool material, std::string&& material_components
) noexcept
    : verbal(verbal), somatic(somatic), material(material), material_components(std::move(material_components)) {}

bool SpellComponents::has_verbal() const noexcept { return verbal; }

bool SpellComponents::has_somatic() const noexcept { return somatic; }

bool SpellComponents::has_material() const noexcept { return material; }

const std::string& SpellComponents::get_material_components() const noexcept { return material_components; }

std::string SpellComponents::str() const {
    if (!material || material_components.empty()) {
        return short_str();
    }
    return fmt::format("{} ({})", short_str(), material_components);
}

std::string SpellComponents::short_str() const {
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

} // namespace dnd
