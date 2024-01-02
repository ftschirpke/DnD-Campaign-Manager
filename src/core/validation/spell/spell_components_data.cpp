#include <dnd_config.hpp>

#include "spell_components_data.hpp"

#include <regex>
#include <string>

#include <fmt/format.h>

#include <core/errors/errors.hpp>
#include <core/models/spell/spell_components.hpp>

namespace dnd {

static constexpr const char*
    spell_components_regex_cstr = "V, S, M \\(.*\\)|V, S|V, M \\(.*\\)|S, M \\(.*\\)|V|S|M \\(.*\\)";

Errors validate_spell_components(const SpellComponents::Data& data) {
    DND_MEASURE_FUNCTION();
    static const std::regex spell_components_regex(spell_components_regex_cstr);
    Errors errors;
    if (!std::regex_match(data.str, spell_components_regex)) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, fmt::format("Invalid spell components \"{}\"", data.str)
        );
    }
    return errors;
}

} // namespace dnd
