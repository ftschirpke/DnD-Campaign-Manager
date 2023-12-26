#include <dnd_config.hpp>

#include <core/basic_mechanics/dice.hpp>

#include <catch2/catch_test_macros.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][basic_mechanics][dice]";

TEST_CASE("dice_to_int", tags) {
    REQUIRE(dice_to_int(Dice::D4) == 4);
    REQUIRE(dice_to_int(Dice::D6) == 6);
    REQUIRE(dice_to_int(Dice::D8) == 8);
    REQUIRE(dice_to_int(Dice::D10) == 10);
    REQUIRE(dice_to_int(Dice::D12) == 12);
    REQUIRE(dice_to_int(Dice::D20) == 20);
    REQUIRE(dice_to_int(Dice::D100) == 100);
}

TEST_CASE("int_to_dice", tags) {
    REQUIRE(int_to_dice(4) == Dice::D4);
    REQUIRE(int_to_dice(6) == Dice::D6);
    REQUIRE(int_to_dice(8) == Dice::D8);
    REQUIRE(int_to_dice(10) == Dice::D10);
    REQUIRE(int_to_dice(12) == Dice::D12);
    REQUIRE(int_to_dice(20) == Dice::D20);
    REQUIRE(int_to_dice(100) == Dice::D100);
}

TEST_CASE("dice_to_string", tags) {
    REQUIRE(dice_to_string(Dice::D4) == "d4");
    REQUIRE(dice_to_string(Dice::D6) == "d6");
    REQUIRE(dice_to_string(Dice::D8) == "d8");
    REQUIRE(dice_to_string(Dice::D10) == "d10");
    REQUIRE(dice_to_string(Dice::D12) == "d12");
    REQUIRE(dice_to_string(Dice::D20) == "d20");
    REQUIRE(dice_to_string(Dice::D100) == "d100");
}

TEST_CASE("string_to_dice", tags) {
    REQUIRE(string_to_dice("d4") == Dice::D4);
    REQUIRE(string_to_dice("D4") == Dice::D4);
    REQUIRE(string_to_dice("d6") == Dice::D6);
    REQUIRE(string_to_dice("D6") == Dice::D6);
    REQUIRE(string_to_dice("d8") == Dice::D8);
    REQUIRE(string_to_dice("D8") == Dice::D8);
    REQUIRE(string_to_dice("d10") == Dice::D10);
    REQUIRE(string_to_dice("D10") == Dice::D10);
    REQUIRE(string_to_dice("d12") == Dice::D12);
    REQUIRE(string_to_dice("D12") == Dice::D12);
    REQUIRE(string_to_dice("d20") == Dice::D20);
    REQUIRE(string_to_dice("D20") == Dice::D20);
    REQUIRE(string_to_dice("d100") == Dice::D100);
    REQUIRE(string_to_dice("D100") == Dice::D100);
}

TEST_CASE("value_is_possible_for", tags) {
    SECTION("d4") {
        REQUIRE_FALSE(value_is_possible_for(-1, Dice::D4));
        REQUIRE_FALSE(value_is_possible_for(0, Dice::D4));
        REQUIRE(value_is_possible_for(1, Dice::D4));
        REQUIRE(value_is_possible_for(3, Dice::D4));
        REQUIRE(value_is_possible_for(4, Dice::D4));
        REQUIRE_FALSE(value_is_possible_for(5, Dice::D4));
    }
    SECTION("d6") {
        REQUIRE_FALSE(value_is_possible_for(-1, Dice::D6));
        REQUIRE_FALSE(value_is_possible_for(0, Dice::D6));
        REQUIRE(value_is_possible_for(1, Dice::D6));
        REQUIRE(value_is_possible_for(4, Dice::D6));
        REQUIRE(value_is_possible_for(6, Dice::D6));
        REQUIRE_FALSE(value_is_possible_for(7, Dice::D6));
    }
    SECTION("d8") {
        REQUIRE_FALSE(value_is_possible_for(-1, Dice::D8));
        REQUIRE_FALSE(value_is_possible_for(0, Dice::D8));
        REQUIRE(value_is_possible_for(1, Dice::D8));
        REQUIRE(value_is_possible_for(3, Dice::D8));
        REQUIRE(value_is_possible_for(5, Dice::D8));
        REQUIRE(value_is_possible_for(8, Dice::D8));
        REQUIRE_FALSE(value_is_possible_for(9, Dice::D8));
    }
    SECTION("d10") {
        REQUIRE_FALSE(value_is_possible_for(-1, Dice::D10));
        REQUIRE_FALSE(value_is_possible_for(0, Dice::D10));
        REQUIRE(value_is_possible_for(1, Dice::D10));
        REQUIRE(value_is_possible_for(2, Dice::D10));
        REQUIRE(value_is_possible_for(7, Dice::D10));
        REQUIRE(value_is_possible_for(10, Dice::D10));
        REQUIRE_FALSE(value_is_possible_for(11, Dice::D10));
    }
    SECTION("d12") {
        REQUIRE_FALSE(value_is_possible_for(-1, Dice::D12));
        REQUIRE_FALSE(value_is_possible_for(0, Dice::D12));
        REQUIRE(value_is_possible_for(1, Dice::D12));
        REQUIRE(value_is_possible_for(2, Dice::D12));
        REQUIRE(value_is_possible_for(5, Dice::D12));
        REQUIRE(value_is_possible_for(10, Dice::D12));
        REQUIRE(value_is_possible_for(12, Dice::D12));
        REQUIRE_FALSE(value_is_possible_for(13, Dice::D12));
    }
    SECTION("d20") {
        REQUIRE_FALSE(value_is_possible_for(-1, Dice::D20));
        REQUIRE_FALSE(value_is_possible_for(0, Dice::D20));
        REQUIRE(value_is_possible_for(1, Dice::D20));
        REQUIRE(value_is_possible_for(3, Dice::D20));
        REQUIRE(value_is_possible_for(7, Dice::D20));
        REQUIRE(value_is_possible_for(12, Dice::D20));
        REQUIRE(value_is_possible_for(19, Dice::D20));
        REQUIRE(value_is_possible_for(20, Dice::D20));
        REQUIRE_FALSE(value_is_possible_for(21, Dice::D20));
    }
    SECTION("d100") {
        REQUIRE_FALSE(value_is_possible_for(-1, Dice::D100));
        REQUIRE_FALSE(value_is_possible_for(0, Dice::D100));
        REQUIRE(value_is_possible_for(1, Dice::D100));
        REQUIRE(value_is_possible_for(3, Dice::D100));
        REQUIRE(value_is_possible_for(19, Dice::D100));
        REQUIRE(value_is_possible_for(47, Dice::D100));
        REQUIRE(value_is_possible_for(60, Dice::D100));
        REQUIRE(value_is_possible_for(92, Dice::D100));
        REQUIRE(value_is_possible_for(100, Dice::D100));
        REQUIRE_FALSE(value_is_possible_for(101, Dice::D100));
    }
}

} // namespace dnd::test
