#ifndef SPELL_HPP_
#define SPELL_HPP_

#include <string>

namespace dnd {
    
class Spell {
public:
    const std::string name;
    Spell(const std::string& name);
};

} // namespace dnd

#endif // SPELL_HPP_
