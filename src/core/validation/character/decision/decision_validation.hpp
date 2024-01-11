#ifndef DECISION_VALIDATION_HPP_
#define DECISION_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <core/errors/errors.hpp>
#include <core/models/character/character.hpp>
#include <core/models/character/decision/decision.hpp>

namespace dnd {

class Content;
class Effects;

Errors validate_decision_for_character_and_content(
    const Decision::Data& data, const Character::Data& character_validation, const Content& content
);

} // namespace dnd

#endif // DECISION_VALIDATION_HPP_
