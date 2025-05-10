#ifndef DECISION_VALIDATION_HPP_
#define DECISION_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <core/data_result.hpp>
#include <core/errors/errors.hpp>
#include <core/models/character/character.hpp>

namespace dnd {

class Content;
class Effects;

Errors validate_decision_for_content(const Decision::Data& data, const Content& content);

} // namespace dnd

#endif // DECISION_VALIDATION_HPP_
