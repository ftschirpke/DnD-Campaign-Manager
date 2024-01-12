#include <dnd_config.hpp>

#include "spell_components.hpp"

#include <cassert>
#include <string>
#include <utility>

#include <fmt/format.h>

#include <core/errors/errors.hpp>
#include <core/errors/runtime_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>
#include <core/validation/spell/spell_components_validation.hpp>

namespace dnd {

CreateResult<SpellComponents> SpellComponents::create(Data&& data) {
    Errors errors = validate_spell_components(data);
    if (!errors.ok()) {
        return InvalidCreate<SpellComponents>(std::move(data), std::move(errors));
    }
    bool verbal, somatic, material;
    std::string materials_needed;

    size_t parentheses_idx = data.str.find(" (");
    std::string first_part = (parentheses_idx == std::string::npos) ? data.str : data.str.substr(0, parentheses_idx);
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
        Errors unreachable_errors(RuntimeError(
            RuntimeError::Code::UNREACHABLE,
            fmt::format("Invalid spell components string {} passed validation.", data.str)
        ));
        return InvalidCreate<SpellComponents>(std::move(data), std::move(unreachable_errors));
    }
    if (material) {
        materials_needed = data.str.substr(parentheses_idx + 2, data.str.size() - parentheses_idx - 3);
    }
    return ValidCreate(SpellComponents(verbal, somatic, material, std::move(materials_needed)));
}

SpellComponents::SpellComponents(bool verbal, bool somatic, bool material, const std::string& material_components)
    : verbal(verbal), somatic(somatic), material(material), material_components(material_components) {}

SpellComponents::SpellComponents(bool verbal, bool somatic, bool material, std::string&& material_components)
    : verbal(verbal), somatic(somatic), material(material), material_components(std::move(material_components)) {}

bool SpellComponents::has_verbal() const { return verbal; }

bool SpellComponents::has_somatic() const { return somatic; }

bool SpellComponents::has_material() const { return material; }

const std::string& SpellComponents::get_material_components() const { return material_components; }

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
