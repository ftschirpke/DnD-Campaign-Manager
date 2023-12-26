#ifndef CHARACTER_BASIS_HPP_
#define CHARACTER_BASIS_HPP_

#include <dnd_config.hpp>

#include <functional>
#include <vector>

#include <core/utils/types.hpp>
#include <core/validation/character/character_basis_data.hpp>

namespace dnd {

class Content;
class CharacterSpecies;
class CharacterSubspecies;
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
    static CharacterBasis create(CharacterBasisData&& data, const Content& content);

    const CharacterSpecies& get_species() const noexcept;
    OptCRef<CharacterSubspecies> get_subspecies() const noexcept;
    const CharacterClass& get_class() const noexcept;
    OptCRef<CharacterSubclass> get_subclass() const noexcept;

    bool has_subspecies() const noexcept;
    bool has_subclass() const noexcept;
private:
    CharacterBasis(
        const CharacterSpecies& species, OptCRef<CharacterSubspecies> subspecies, const CharacterClass& cls,
        OptCRef<CharacterSubclass> subclass
    ) noexcept;

    std::reference_wrapper<const CharacterSpecies> species;
    OptCRef<CharacterSubspecies> subspecies;
    std::reference_wrapper<const CharacterClass> cls;
    OptCRef<CharacterSubclass> subclass;
};

} // namespace dnd

#endif // CHARACTER_BASIS_HPP_
