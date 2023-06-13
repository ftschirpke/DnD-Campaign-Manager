#ifndef CHARACTER_BASIS_HPP_
#define CHARACTER_BASIS_HPP_

#include <dnd_config.hpp>

#include <core/validation/character/character_basis_data.hpp>

namespace dnd {

class ContentHolder;
class CharacterRace;
class CharacterSubrace;
class CharacterClass;
class CharacterSubclass;

class CharacterBasis {
public:
    /**
     * @brief Constructs a CharacterBasis object
     * @param data the data to construct the CharacterBasis object from
     * @param content the content to use for the construction
     * @return the constructed CharacterBasis object
     * @throws dnd::invalid_data if the given data is invalid or is incompatible with the given content
     */
    static CharacterBasis create(CharacterBasisData&& data, const ContentHolder* content);

    const CharacterRace* get_race() const noexcept;
    const CharacterSubrace* get_subrace() const noexcept;
    const CharacterClass* get_class() const noexcept;
    const CharacterSubclass* get_subclass() const noexcept;

    bool has_subrace() const noexcept;
    bool has_subclass() const noexcept;
private:
    CharacterBasis(
        const CharacterRace* race, const CharacterSubrace* subrace, const CharacterClass* cls,
        const CharacterSubclass* subclass
    ) noexcept;
    CharacterBasis(const CharacterRace* race, const CharacterClass* cls, const CharacterSubclass* subclass) noexcept;
    CharacterBasis(const CharacterRace* race, const CharacterClass* cls) noexcept;

    const CharacterRace* race;
    const CharacterSubrace* subrace;
    const CharacterClass* cls;
    const CharacterSubclass* subclass;
};

} // namespace dnd

#endif // CHARACTER_BASIS_HPP_
