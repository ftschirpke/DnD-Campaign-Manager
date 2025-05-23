#ifndef SPELL_CARD_BUILDER_HPP_
#define SPELL_CARD_BUILDER_HPP_

#include <dnd_config.hpp>

#include <vector>

#include <core/models/spell/spell.hpp>
#include <core/types.hpp>

namespace dnd {

class SpellCardBuilder {
public:
    /**
     * @brief Add to the list of spells we want to create cards for
     * @param spell a spell to create a card for
     */
    void add_spell(const Spell& spell);
    /**
     * @brief Add to the list of spells we want to create cards for
     * @param spell a spell to create a card for
     */
    void add_spell(CRef<Spell> spell);
    std::vector<CRef<Spell>> get_spells() const;
    void clear_spells();
    /**
     * @brief Creates a LaTeX file that allows printing the cards with the timestamp as the file name
     */
    void write_latex_file();
    /**
     * @brief Creates a LaTeX file that allows printing the cards with a given file name
     * @param filename the name of the file
     */
    void write_latex_file(const std::string& filename);
private:
    std::vector<CRef<Spell>> spells;
};

} // namespace dnd

#endif // SPELL_CARD_BUILDER_HPP_
