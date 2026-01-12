#ifndef SPELL_CARD_BUILDER_HPP_
#define SPELL_CARD_BUILDER_HPP_

#include <dnd_config.hpp>

#include <vector>

#include <core/models/spell/spell.hpp>
#include <core/types.hpp>

namespace dnd {

class Content;

class SpellCardBuilder {
public:
    explicit SpellCardBuilder(const Content& content) noexcept;
    void add_spell(Id spell_id);
    std::vector<Id> get_spells() const;
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
    const Content& content;
    std::vector<Id> spells;
};

} // namespace dnd

#endif // SPELL_CARD_BUILDER_HPP_
