#include <dnd_config.hpp>

#include "spell_type_data.hpp"

#include <regex>
#include <string>

#include <fmt/format.h>

#include <core/errors/errors.hpp>
#include <core/errors/validation_error.hpp>
#include <core/validation/validation_data.hpp>
#include <core/validation/validation_subdata.hpp>

static constexpr const char* spell_type_regex_cstr = "V, S, M \\(.*\\)|V, S|V, M \\(.*\\)|S, M \\(.*\\)|V|S|M \\(.*\\)";

dnd::SpellTypeData::SpellTypeData(const dnd::ValidationData* parent) noexcept : ValidationSubdata(parent) {}

dnd::Errors dnd::SpellTypeData::validate() const {
    DND_MEASURE_FUNCTION();
    static const std::regex spell_type_regex(spell_type_regex_cstr);
    Errors errors;
    if (!std::regex_match(str, spell_type_regex)) {
        errors.add_validation_error(
            ValidationErrorCode::INVALID_ATTRIBUTE_VALUE, parent, fmt::format("Invalid spell type \"{}\"", str)
        );
    }
    return errors;
}
