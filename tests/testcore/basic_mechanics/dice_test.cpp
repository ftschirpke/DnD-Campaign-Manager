#include <dnd_config.hpp>

#include <core/basic_mechanics/dice.hpp>

#include <catch2/catch_test_macros.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][basic_mechanics][dice]";

TEST_CASE("Dice::max_value", tags) {
    REQUIRE(Dice::single_from_int(4).max_value() == 4);
    REQUIRE(Dice::single_from_int(6).max_value() == 6);
    REQUIRE(Dice::single_from_int(8).max_value() == 8);
    REQUIRE(Dice::single_from_int(10).max_value() == 10);
    REQUIRE(Dice::single_from_int(12).max_value() == 12);
    REQUIRE(Dice::single_from_int(20).max_value() == 20);
    REQUIRE(Dice::single_from_int(100).max_value() == 100);
}

TEST_CASE("Dice::to_string", tags) {
    REQUIRE(Dice::single_from_int(4).to_string() == "1d4");
    REQUIRE(Dice::single_from_int(6).to_string() == "1d6");
    REQUIRE(Dice::single_from_int(8).to_string() == "1d8");
    REQUIRE(Dice::single_from_int(10).to_string() == "1d10");
    REQUIRE(Dice::single_from_int(12).to_string() == "1d12");
    REQUIRE(Dice::single_from_int(20).to_string() == "1d20");
    REQUIRE(Dice::single_from_int(100).to_string() == "1d100");
}

TEST_CASE("Dice::from_string", tags) {
    REQUIRE(Dice::from_string("d4").max_value() == 4);
    REQUIRE(Dice::from_string("D4").max_value() == 4);
    REQUIRE(Dice::from_string("d6").max_value() == 6);
    REQUIRE(Dice::from_string("D6").max_value() == 6);
    REQUIRE(Dice::from_string("d8").max_value() == 8);
    REQUIRE(Dice::from_string("D8").max_value() == 8);
    REQUIRE(Dice::from_string("d10").max_value() == 10);
    REQUIRE(Dice::from_string("D10").max_value() == 10);
    REQUIRE(Dice::from_string("d12").max_value() == 12);
    REQUIRE(Dice::from_string("D12").max_value() == 12);
    REQUIRE(Dice::from_string("d20").max_value() == 20);
    REQUIRE(Dice::from_string("D20").max_value() == 20);
    REQUIRE(Dice::from_string("d100").max_value() == 100);
    REQUIRE(Dice::from_string("D100").max_value() == 100);

    REQUIRE(Dice::from_string("1d4+2d6").max_value() == 16);
    REQUIRE(Dice::from_string("4d10-10").min_value() == -6);
    REQUIRE(Dice::from_string("4d10-10").max_value() == 30);
}

TEST_CASE("value_is_possible_for", tags) {
    SECTION("d4") {
        Dice d4 = Dice::single_from_int(4);
        REQUIRE_FALSE(d4.value_is_possible(-1));
        REQUIRE_FALSE(d4.value_is_possible(0));
        REQUIRE(d4.value_is_possible(1));
        REQUIRE(d4.value_is_possible(3));
        REQUIRE(d4.value_is_possible(4));
        REQUIRE_FALSE(d4.value_is_possible(5));
    }
    SECTION("d6") {
        Dice d6 = Dice::single_from_int(6);
        REQUIRE_FALSE(d6.value_is_possible(-1));
        REQUIRE_FALSE(d6.value_is_possible(0));
        REQUIRE(d6.value_is_possible(1));
        REQUIRE(d6.value_is_possible(4));
        REQUIRE(d6.value_is_possible(6));
        REQUIRE_FALSE(d6.value_is_possible(7));
    }
    SECTION("d8") {
        Dice d8 = Dice::single_from_int(8);
        REQUIRE_FALSE(d8.value_is_possible(-1));
        REQUIRE_FALSE(d8.value_is_possible(0));
        REQUIRE(d8.value_is_possible(1));
        REQUIRE(d8.value_is_possible(3));
        REQUIRE(d8.value_is_possible(5));
        REQUIRE(d8.value_is_possible(8));
        REQUIRE_FALSE(d8.value_is_possible(9));
    }
    SECTION("d10") {
        Dice d10 = Dice::single_from_int(10);
        REQUIRE_FALSE(d10.value_is_possible(-1));
        REQUIRE_FALSE(d10.value_is_possible(0));
        REQUIRE(d10.value_is_possible(1));
        REQUIRE(d10.value_is_possible(2));
        REQUIRE(d10.value_is_possible(7));
        REQUIRE(d10.value_is_possible(10));
        REQUIRE_FALSE(d10.value_is_possible(11));
    }
    SECTION("d12") {
        Dice d12 = Dice::single_from_int(12);
        REQUIRE_FALSE(d12.value_is_possible(-1));
        REQUIRE_FALSE(d12.value_is_possible(0));
        REQUIRE(d12.value_is_possible(1));
        REQUIRE(d12.value_is_possible(2));
        REQUIRE(d12.value_is_possible(5));
        REQUIRE(d12.value_is_possible(10));
        REQUIRE(d12.value_is_possible(12));
        REQUIRE_FALSE(d12.value_is_possible(13));
    }
    SECTION("d20") {
        Dice d20 = Dice::single_from_int(20);
        REQUIRE_FALSE(d20.value_is_possible(-1));
        REQUIRE_FALSE(d20.value_is_possible(0));
        REQUIRE(d20.value_is_possible(1));
        REQUIRE(d20.value_is_possible(3));
        REQUIRE(d20.value_is_possible(7));
        REQUIRE(d20.value_is_possible(12));
        REQUIRE(d20.value_is_possible(19));
        REQUIRE(d20.value_is_possible(20));
        REQUIRE_FALSE(d20.value_is_possible(21));
    }
    SECTION("d100") {
        Dice d100 = Dice::single_from_int(100);
        REQUIRE_FALSE(d100.value_is_possible(-1));
        REQUIRE_FALSE(d100.value_is_possible(0));
        REQUIRE(d100.value_is_possible(1));
        REQUIRE(d100.value_is_possible(3));
        REQUIRE(d100.value_is_possible(19));
        REQUIRE(d100.value_is_possible(47));
        REQUIRE(d100.value_is_possible(60));
        REQUIRE(d100.value_is_possible(92));
        REQUIRE(d100.value_is_possible(100));
        REQUIRE_FALSE(d100.value_is_possible(101));
    }
}

} // namespace dnd::test
