#ifndef CONTENT_CONTROLLER_HPP_
#define CONTENT_CONTROLLER_HPP_

#include <string>
#include <unordered_map>

#include "spell.hpp"

namespace dnd {
    
class ContentController {
public:
    std::unordered_map<std::string, Spell> spells;
};

} // namespace dnd

#endif // CONTENT_CONTROLLER_HPP_
