#ifndef EXTRA_SPELLS_HOLDER_HPP_
#define EXTRA_SPELLS_HOLDER_HPP_

#include <string>
#include <unordered_set>

namespace dnd {

class ExtraSpellsHolder {
public:
    std::unordered_set<std::string> free_cantrips, spells_at_will, innate_spells;
};

} // namespace dnd

#endif // EXTRA_SPELLS_HOLDER_HPP_
