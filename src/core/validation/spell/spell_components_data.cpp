#include <dnd_config.hpp>

#include "spell_components_data.hpp"

#include <regex>
#include <string>

#include <fmt/format.h>

#include <core/errors/errors.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

namespace dnd {

SpellComponentsData::SpellComponentsData(std::shared_ptr<const ValidationData> parent) noexcept
    : ValidationSubdata(parent) {}

static constexpr const char*
    spell_components_regex_cstr = "V, S, M \\(.*\\)|V, S|V, M \\(.*\\)|S, M \\(.*\\)|V|S|M \\(.*\\)";

Errors validate_spell_components(const SpellComponentsData& data) {
    DND_MEASURE_FUNCTION();
    static const std::regex spell_components_regex(spell_components_regex_cstr);
    Errors errors;
    if (!std::regex_match(data.str, spell_components_regex)) {
        errors.add_validation_error(
            ValidationError::Code::INVALID_ATTRIBUTE_VALUE, data.get_parent(),
            fmt::format("Invalid spell components \"{}\"", data.str)
        );
    }
    return errors;
}

} // namespace dnd
