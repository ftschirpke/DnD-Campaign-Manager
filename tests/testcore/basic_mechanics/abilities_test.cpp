#include <dnd_config.hpp>

#include <core/basic_mechanics/abilities.hpp>

#include <catch2/catch_test_macros.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][basic_mechanics]";

TEST_CASE("string_to_ability", tags) {
    SECTION("Valid ability strings") {
        REQUIRE(ability_from_string("str").value() == Ability::STRENGTH);
        REQUIRE(ability_from_string("dex").value() == Ability::DEXTERITY);
        REQUIRE(ability_from_string("con").value() == Ability::CONSTITUTION);
        REQUIRE(ability_from_string("int").value() == Ability::INTELLIGENCE);
        REQUIRE(ability_from_string("wis").value() == Ability::WISDOM);
        REQUIRE(ability_from_string("cha").value() == Ability::CHARISMA);
    }

    SECTION("Invalid ability strings") {
        REQUIRE_FALSE(ability_from_string("xyz").has_value());
        REQUIRE_FALSE(ability_from_string("abcd").has_value());
        REQUIRE_FALSE(ability_from_string("").has_value());
    }
}

TEST_CASE("ability_to_string", tags) {
    REQUIRE(ability_name(Ability::STRENGTH) == "str");
    REQUIRE(ability_name(Ability::DEXTERITY) == "dex");
    REQUIRE(ability_name(Ability::CONSTITUTION) == "con");
    REQUIRE(ability_name(Ability::INTELLIGENCE) == "int");
    REQUIRE(ability_name(Ability::WISDOM) == "wis");
    REQUIRE(ability_name(Ability::CHARISMA) == "cha");
}

TEST_CASE("is_ability // string_view implementation", tags) {
    std::string_view valid_ability1 = "dex";
    std::string_view valid_ability2 = "con";
    std::string_view invalid_ability = "xyz";
    std::string_view empty_string;

    REQUIRE(is_ability(valid_ability1));
    REQUIRE(is_ability(valid_ability2));
    REQUIRE_FALSE(is_ability(invalid_ability));
    REQUIRE_FALSE(is_ability(empty_string));
}

TEST_CASE("is_ability // string implementation", tags) {
    std::string valid_ability1 = "str";
    std::string valid_ability2 = "cha";
    std::string invalid_ability = "abcd";
    std::string empty_string;

    REQUIRE(is_ability(valid_ability1));
    REQUIRE(is_ability(valid_ability2));
    REQUIRE_FALSE(is_ability(invalid_ability));
    REQUIRE_FALSE(is_ability(empty_string));
}

} // namespace dnd::test
