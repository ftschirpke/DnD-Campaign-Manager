#include <core/basic_mechanics/dice.hpp>

#include <catch2/catch_test_macros.hpp>

static constexpr const char* tags = "[core][basic_mechanics][dice]";

TEST_CASE("dnd::dice_to_int", tags) {
    REQUIRE(dnd::dice_to_int(dnd::Dice::D4) == 4);
    REQUIRE(dnd::dice_to_int(dnd::Dice::D6) == 6);
    REQUIRE(dnd::dice_to_int(dnd::Dice::D8) == 8);
    REQUIRE(dnd::dice_to_int(dnd::Dice::D10) == 10);
    REQUIRE(dnd::dice_to_int(dnd::Dice::D12) == 12);
    REQUIRE(dnd::dice_to_int(dnd::Dice::D20) == 20);
    REQUIRE(dnd::dice_to_int(dnd::Dice::D100) == 100);
}

TEST_CASE("dnd::int_to_dice", tags) {
    REQUIRE(dnd::int_to_dice(4) == dnd::Dice::D4);
    REQUIRE(dnd::int_to_dice(6) == dnd::Dice::D6);
    REQUIRE(dnd::int_to_dice(8) == dnd::Dice::D8);
    REQUIRE(dnd::int_to_dice(10) == dnd::Dice::D10);
    REQUIRE(dnd::int_to_dice(12) == dnd::Dice::D12);
    REQUIRE(dnd::int_to_dice(20) == dnd::Dice::D20);
    REQUIRE(dnd::int_to_dice(100) == dnd::Dice::D100);
}

TEST_CASE("dnd::dice_to_string", tags) {
    REQUIRE(dnd::dice_to_string(dnd::Dice::D4) == "d4");
    REQUIRE(dnd::dice_to_string(dnd::Dice::D6) == "d6");
    REQUIRE(dnd::dice_to_string(dnd::Dice::D8) == "d8");
    REQUIRE(dnd::dice_to_string(dnd::Dice::D10) == "d10");
    REQUIRE(dnd::dice_to_string(dnd::Dice::D12) == "d12");
    REQUIRE(dnd::dice_to_string(dnd::Dice::D20) == "d20");
    REQUIRE(dnd::dice_to_string(dnd::Dice::D100) == "d100");
}

TEST_CASE("dnd::string_to_dice", tags) {
    REQUIRE(dnd::string_to_dice("d4") == dnd::Dice::D4);
    REQUIRE(dnd::string_to_dice("D4") == dnd::Dice::D4);
    REQUIRE(dnd::string_to_dice("d6") == dnd::Dice::D6);
    REQUIRE(dnd::string_to_dice("D6") == dnd::Dice::D6);
    REQUIRE(dnd::string_to_dice("d8") == dnd::Dice::D8);
    REQUIRE(dnd::string_to_dice("D8") == dnd::Dice::D8);
    REQUIRE(dnd::string_to_dice("d10") == dnd::Dice::D10);
    REQUIRE(dnd::string_to_dice("D10") == dnd::Dice::D10);
    REQUIRE(dnd::string_to_dice("d12") == dnd::Dice::D12);
    REQUIRE(dnd::string_to_dice("D12") == dnd::Dice::D12);
    REQUIRE(dnd::string_to_dice("d20") == dnd::Dice::D20);
    REQUIRE(dnd::string_to_dice("D20") == dnd::Dice::D20);
    REQUIRE(dnd::string_to_dice("d100") == dnd::Dice::D100);
    REQUIRE(dnd::string_to_dice("D100") == dnd::Dice::D100);
}

TEST_CASE("dnd::value_is_possible_for", tags) {
    SECTION("d4") {
        REQUIRE_FALSE(dnd::value_is_possible_for(-1, dnd::Dice::D4));
        REQUIRE_FALSE(dnd::value_is_possible_for(0, dnd::Dice::D4));
        REQUIRE(dnd::value_is_possible_for(1, dnd::Dice::D4));
        REQUIRE(dnd::value_is_possible_for(3, dnd::Dice::D4));
        REQUIRE(dnd::value_is_possible_for(4, dnd::Dice::D4));
        REQUIRE_FALSE(dnd::value_is_possible_for(5, dnd::Dice::D4));
    }
    SECTION("d6") {
        REQUIRE_FALSE(dnd::value_is_possible_for(-1, dnd::Dice::D6));
        REQUIRE_FALSE(dnd::value_is_possible_for(0, dnd::Dice::D6));
        REQUIRE(dnd::value_is_possible_for(1, dnd::Dice::D6));
        REQUIRE(dnd::value_is_possible_for(4, dnd::Dice::D6));
        REQUIRE(dnd::value_is_possible_for(6, dnd::Dice::D6));
        REQUIRE_FALSE(dnd::value_is_possible_for(7, dnd::Dice::D6));
    }
    SECTION("d8") {
        REQUIRE_FALSE(dnd::value_is_possible_for(-1, dnd::Dice::D8));
        REQUIRE_FALSE(dnd::value_is_possible_for(0, dnd::Dice::D8));
        REQUIRE(dnd::value_is_possible_for(1, dnd::Dice::D8));
        REQUIRE(dnd::value_is_possible_for(3, dnd::Dice::D8));
        REQUIRE(dnd::value_is_possible_for(5, dnd::Dice::D8));
        REQUIRE(dnd::value_is_possible_for(8, dnd::Dice::D8));
        REQUIRE_FALSE(dnd::value_is_possible_for(9, dnd::Dice::D8));
    }
    SECTION("d10") {
        REQUIRE_FALSE(dnd::value_is_possible_for(-1, dnd::Dice::D10));
        REQUIRE_FALSE(dnd::value_is_possible_for(0, dnd::Dice::D10));
        REQUIRE(dnd::value_is_possible_for(1, dnd::Dice::D10));
        REQUIRE(dnd::value_is_possible_for(2, dnd::Dice::D10));
        REQUIRE(dnd::value_is_possible_for(7, dnd::Dice::D10));
        REQUIRE(dnd::value_is_possible_for(10, dnd::Dice::D10));
        REQUIRE_FALSE(dnd::value_is_possible_for(11, dnd::Dice::D10));
    }
    SECTION("d12") {
        REQUIRE_FALSE(dnd::value_is_possible_for(-1, dnd::Dice::D12));
        REQUIRE_FALSE(dnd::value_is_possible_for(0, dnd::Dice::D12));
        REQUIRE(dnd::value_is_possible_for(1, dnd::Dice::D12));
        REQUIRE(dnd::value_is_possible_for(2, dnd::Dice::D12));
        REQUIRE(dnd::value_is_possible_for(5, dnd::Dice::D12));
        REQUIRE(dnd::value_is_possible_for(10, dnd::Dice::D12));
        REQUIRE(dnd::value_is_possible_for(12, dnd::Dice::D12));
        REQUIRE_FALSE(dnd::value_is_possible_for(13, dnd::Dice::D12));
    }
    SECTION("d20") {
        REQUIRE_FALSE(dnd::value_is_possible_for(-1, dnd::Dice::D20));
        REQUIRE_FALSE(dnd::value_is_possible_for(0, dnd::Dice::D20));
        REQUIRE(dnd::value_is_possible_for(1, dnd::Dice::D20));
        REQUIRE(dnd::value_is_possible_for(3, dnd::Dice::D20));
        REQUIRE(dnd::value_is_possible_for(7, dnd::Dice::D20));
        REQUIRE(dnd::value_is_possible_for(12, dnd::Dice::D20));
        REQUIRE(dnd::value_is_possible_for(19, dnd::Dice::D20));
        REQUIRE(dnd::value_is_possible_for(20, dnd::Dice::D20));
        REQUIRE_FALSE(dnd::value_is_possible_for(21, dnd::Dice::D20));
    }
    SECTION("d100") {
        REQUIRE_FALSE(dnd::value_is_possible_for(-1, dnd::Dice::D100));
        REQUIRE_FALSE(dnd::value_is_possible_for(0, dnd::Dice::D100));
        REQUIRE(dnd::value_is_possible_for(1, dnd::Dice::D100));
        REQUIRE(dnd::value_is_possible_for(3, dnd::Dice::D100));
        REQUIRE(dnd::value_is_possible_for(19, dnd::Dice::D100));
        REQUIRE(dnd::value_is_possible_for(47, dnd::Dice::D100));
        REQUIRE(dnd::value_is_possible_for(60, dnd::Dice::D100));
        REQUIRE(dnd::value_is_possible_for(92, dnd::Dice::D100));
        REQUIRE(dnd::value_is_possible_for(100, dnd::Dice::D100));
        REQUIRE_FALSE(dnd::value_is_possible_for(101, dnd::Dice::D100));
    }
}
