#include <dnd_config.hpp>

#include <core/models/character_class/character_class.hpp>

#include <cassert>

#include <catch2/catch_test_macros.hpp>

#include <core/content.hpp>
#include <core/models/character/character.hpp>
#include <core/models/character/character_basis.hpp>
#include <testcore/minimal_testing_content.hpp>
#include <variant>


static constexpr const char* tags = "[core][character][iterator]";

TEST_CASE("dnd::CharacterBasis::Iterator", tags) {
    dnd::Content content = dndtest::minimal_testing_content();

    const dnd::Character& character = content.get_characters().get("Example Character").value().get();
    const dnd::CharacterBasis& basis = character.get_basis();

    dnd::CharacterBasis::Iterator it = basis.begin();
    dnd::CharacterBasis::Iterator end_it = basis.end();

    REQUIRE(it != end_it);

    REQUIRE(it->index() == 0);
    REQUIRE(std::get<0>(*it).get().get_name() == "Dwarf");
    ++it;
    REQUIRE(it->index() == 1);
    REQUIRE(std::get<1>(*it).get().get_name() == "Hill Dwarf");
    ++it;
    REQUIRE(it->index() == 2);
    REQUIRE(std::get<2>(*it).get().get_name() == "Wizard");
    ++it;
    REQUIRE(it->index() == 3);
    REQUIRE(std::get<3>(*it).get().get_name() == "Abjuration Wizard");
}
