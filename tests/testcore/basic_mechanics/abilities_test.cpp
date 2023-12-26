#include <dnd_config.hpp>

#include <core/basic_mechanics/abilities.hpp>

#include <catch2/catch_test_macros.hpp>

#include <stdexcept>

namespace dnd::test {

static constexpr const char* tags = "[core][basic_mechanics]";

TEST_CASE("string_to_ability", tags) {
    SECTION("Valid ability strings") {
        REQUIRE(string_to_ability("STR") == Ability::STRENGTH);
        REQUIRE(string_to_ability("DEX") == Ability::DEXTERITY);
        REQUIRE(string_to_ability("CON") == Ability::CONSTITUTION);
        REQUIRE(string_to_ability("INT") == Ability::INTELLIGENCE);
        REQUIRE(string_to_ability("WIS") == Ability::WISDOM);
        REQUIRE(string_to_ability("CHA") == Ability::CHARISMA);
    }

    SECTION("Invalid ability strings") {
        REQUIRE_THROWS_AS(string_to_ability("XYZ"), std::invalid_argument);
        REQUIRE_THROWS_AS(string_to_ability("ABCD"), std::invalid_argument);
        REQUIRE_THROWS_AS(string_to_ability(""), std::invalid_argument);
    }
}

TEST_CASE("ability_to_string", tags) {
    REQUIRE(ability_to_string(Ability::STRENGTH) == "STR");
    REQUIRE(ability_to_string(Ability::DEXTERITY) == "DEX");
    REQUIRE(ability_to_string(Ability::CONSTITUTION) == "CON");
    REQUIRE(ability_to_string(Ability::INTELLIGENCE) == "INT");
    REQUIRE(ability_to_string(Ability::WISDOM) == "WIS");
    REQUIRE(ability_to_string(Ability::CHARISMA) == "CHA");
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
