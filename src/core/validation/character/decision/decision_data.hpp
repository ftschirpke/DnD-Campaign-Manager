#ifndef DECISION_VALIDATION_HPP_
#define DECISION_VALIDATION_HPP_

#include <dnd_config.hpp>

#include <compare>
#include <map>
#include <string>
#include <vector>

#include <core/errors/errors.hpp>
#include <core/models/character/character.hpp>
#include <core/models/character/decision/decision.hpp>
#include <core/utils/types.hpp>

namespace dnd {

class Content;
class Effects;

Errors validate_decision_for_character_and_content(
    const Decision::Data& data, const Character::Data& character_data, const Content& content
);

} // namespace dnd

#endif // DECISION_VALIDATION_HPP_
