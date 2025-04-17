#include <dnd_config.hpp>

#include "spell_components.hpp"

#include <cassert>
#include <string>
#include <utility>

#include <fmt/format.h>

#include <core/errors/errors.hpp>
#include <core/errors/runtime_error.hpp>
#include <core/exceptions/validation_exceptions.hpp>

namespace dnd {

CreateResult<SpellComponents> SpellComponents::create(Data&& data) {
    bool material = !data.material_components.empty();
    return ValidCreate(SpellComponents(data.verbal, data.somatic, material, std::move(data.material_components)));
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
