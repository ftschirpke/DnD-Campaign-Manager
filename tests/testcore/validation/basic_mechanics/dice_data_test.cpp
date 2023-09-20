#include <dnd_config.hpp>

#include <core/validation/basic_mechanics/dice_data.hpp>

#include <catch2/catch_test_macros.hpp>

static constexpr const char* tags = "[core][validation][basic_mechanics][dice]";

TEST_CASE("dnd::DiceData", tags) {
    dnd::DiceData data(nullptr);

    dnd::Errors errors;
    SECTION("Valid dice") {
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

    SECTION("Invalid dice") {
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
