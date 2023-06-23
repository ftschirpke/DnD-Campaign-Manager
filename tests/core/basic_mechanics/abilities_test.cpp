#include <core/basic_mechanics/abilities.hpp>

#include <catch2/catch_test_macros.hpp>

#include <stdexcept>

static constexpr const char* tags = "[core][basic_mechanics]";

TEST_CASE("dnd::string_to_ability", tags) {
    SECTION("Valid ability strings") {
        REQUIRE(dnd::string_to_ability("STR") == dnd::Ability::STRENGTH);
        REQUIRE(dnd::string_to_ability("DEX") == dnd::Ability::DEXTERITY);
        REQUIRE(dnd::string_to_ability("CON") == dnd::Ability::CONSTITUTION);
        REQUIRE(dnd::string_to_ability("INT") == dnd::Ability::INTELLIGENCE);
        REQUIRE(dnd::string_to_ability("WIS") == dnd::Ability::WISDOM);
        REQUIRE(dnd::string_to_ability("CHA") == dnd::Ability::CHARISMA);
    }

    SECTION("Invalid ability strings") {
        REQUIRE_THROWS_AS(dnd::string_to_ability("XYZ"), std::invalid_argument);
        REQUIRE_THROWS_AS(dnd::string_to_ability("ABCD"), std::invalid_argument);
        REQUIRE_THROWS_AS(dnd::string_to_ability(""), std::invalid_argument);
    }
}

TEST_CASE("dnd::ability_to_string", tags) {
    REQUIRE(dnd::ability_to_string(dnd::Ability::STRENGTH) == "STR");
    REQUIRE(dnd::ability_to_string(dnd::Ability::DEXTERITY) == "DEX");
    REQUIRE(dnd::ability_to_string(dnd::Ability::CONSTITUTION) == "CON");
    REQUIRE(dnd::ability_to_string(dnd::Ability::INTELLIGENCE) == "INT");
    REQUIRE(dnd::ability_to_string(dnd::Ability::WISDOM) == "WIS");
    REQUIRE(dnd::ability_to_string(dnd::Ability::CHARISMA) == "CHA");
}

TEST_CASE("dnd::is_ability: string_view implementation", tags) {
    std::string_view valid_ability1 = "DEX";
    std::string_view valid_ability2 = "CON";
    std::string_view invalid_ability = "XYZ";
    std::string_view empty_string;

    REQUIRE(dnd::is_ability(valid_ability1));
    REQUIRE(dnd::is_ability(valid_ability2));
    REQUIRE_FALSE(dnd::is_ability(invalid_ability));
    REQUIRE_FALSE(dnd::is_ability(empty_string));
}

TEST_CASE("dnd::is_ability: std::string implementation", tags) {
    std::string valid_ability1 = "STR";
    std::string valid_ability2 = "CHA";
    std::string invalid_ability = "ABCD";
    std::string empty_string;

    REQUIRE(dnd::is_ability(valid_ability1));
    REQUIRE(dnd::is_ability(valid_ability2));
    REQUIRE_FALSE(dnd::is_ability(invalid_ability));
    REQUIRE_FALSE(dnd::is_ability(empty_string));
}
