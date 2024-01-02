#ifndef PROFICIENCY_HOLDER_VALIDATION_HPP_
#define PROFICIENCY_HOLDER_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <core/errors/errors.hpp>
#include <core/models/effects/subholders/proficiency_holder.hpp>

namespace dnd {

class Content;

Errors validate_proficiency_holder_for_content(const ProficiencyHolder::Data& data, const Content& content);

} // namespace dnd

#endif // PROFICIENCY_HOLDER_VALIDATION_HPP_
