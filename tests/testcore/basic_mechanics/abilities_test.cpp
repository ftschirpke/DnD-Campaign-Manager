#include <dnd_config.hpp>

#include <core/basic_mechanics/abilities.hpp>

#include <catch2/catch_test_macros.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][basic_mechanics]";

TEST_CASE("string_to_ability", tags) {
    SECTION("Valid ability strings") {
        REQUIRE(ability_from_string("STR").value() == Ability::STRENGTH);
        REQUIRE(ability_from_string("DEX").value() == Ability::DEXTERITY);
        REQUIRE(ability_from_string("CON").value() == Ability::CONSTITUTION);
        REQUIRE(ability_from_string("INT").value() == Ability::INTELLIGENCE);
        REQUIRE(ability_from_string("WIS").value() == Ability::WISDOM);
        REQUIRE(ability_from_string("CHA").value() == Ability::CHARISMA);
    }

    SECTION("Invalid ability strings") {
        REQUIRE_FALSE(ability_from_string("XYZ").has_value());
        REQUIRE_FALSE(ability_from_string("ABCD").has_value());
        REQUIRE_FALSE(ability_from_string("").has_value());
    }
}

TEST_CASE("ability_to_string", tags) {
    REQUIRE(ability_name(Ability::STRENGTH) == "STR");
    REQUIRE(ability_name(Ability::DEXTERITY) == "DEX");
    REQUIRE(ability_name(Ability::CONSTITUTION) == "CON");
    REQUIRE(ability_name(Ability::INTELLIGENCE) == "INT");
    REQUIRE(ability_name(Ability::WISDOM) == "WIS");
    REQUIRE(ability_name(Ability::CHARISMA) == "CHA");
}

TEST_CASE("is_ability // string_view implementation", tags) {
    std::string_view valid_ability1 = "DEX";
    std::string_view valid_ability2 = "CON";
    std::string_view invalid_ability = "XYZ";
    std::string_view empty_string;

    REQUIRE(is_ability(valid_ability1));
    REQUIRE(is_ability(valid_ability2));
    REQUIRE_FALSE(is_ability(invalid_ability));
    REQUIRE_FALSE(is_ability(empty_string));
}

TEST_CASE("is_ability // string implementation", tags) {
    std::string valid_ability1 = "STR";
    std::string valid_ability2 = "CHA";
    std::string invalid_ability = "ABCD";
    std::string empty_string;

    REQUIRE(is_ability(valid_ability1));
    REQUIRE(is_ability(valid_ability2));
    REQUIRE_FALSE(is_ability(invalid_ability));
    REQUIRE_FALSE(is_ability(empty_string));
}

} // namespace dnd::test
