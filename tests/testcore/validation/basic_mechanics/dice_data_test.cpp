#include <dnd_config.hpp>

#include <core/validation/basic_mechanics/dice_data.hpp>

#include <catch2/catch_test_macros.hpp>

namespace dnd::test {

static constexpr const char* tags = "[core][validation][basic_mechanics][dice]";

TEST_CASE("DiceData // single die", tags) {
    DiceData data(nullptr);

    Errors errors;
    SECTION("Valid single die") {
        data.str = "d4";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());

        data.str = "D4";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());

        data.str = "d6";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());

        data.str = "D6";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());

        data.str = "d8";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());

        data.str = "D8";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());

        data.str = "d10";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());

        data.str = "D10";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());

        data.str = "d12";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());

        data.str = "D12";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());

        data.str = "d20";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());

        data.str = "D20";
        REQUIRE_NOTHROW(errors = data.validate());

        data.str = "d100";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());

        data.str = "D100";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());
    }

    SECTION("Invalid single die") {
        data.str = "d04";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());

        data.str = "D14";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());

        data.str = "d-1";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());

        data.str = "D010";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());

        data.str = "xyz";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());

        data.str = "";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }
}

TEST_CASE("DiceData // multiple dice", tags) {
    DiceData data(nullptr);

    Errors errors;
    SECTION("Valid single die") {
        data.str = "1d4+2d4";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());

        data.str = "4D4+3";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());

        data.str = "2d6+1";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());

        data.str = "14D6+2d4";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());

        data.str = "4d8-13";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());

        data.str = "8D8+14D20";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());

        data.str = "d10+d4+3";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());

        data.str = "D100+2d12";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());
    }

    SECTION("Invalid multiple dice") {
        data.str = "0d4";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());

        data.str = "14D6-3d4";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());

        data.str = "d6+-4d5";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());

        data.str = "5";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());
    }
}

} // namespace dnd::test
