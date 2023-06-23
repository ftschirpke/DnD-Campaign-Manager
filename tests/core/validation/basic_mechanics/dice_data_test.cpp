#include <dnd_config.hpp>

#include <core/validation/basic_mechanics/dice_data.hpp>

#include <catch2/catch_test_macros.hpp>

static constexpr const char* tags = "[core][validation][basic_mechanics][dice]";

TEST_CASE("dnd::DiceData", tags) {
    dnd::DiceData data(nullptr);

    dnd::Errors errors;
    SECTION("Valid dice") {
        data.str = "d6";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());

        data.str = "d20";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());

        data.str = "d100";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE(errors.ok());
    }

    SECTION("Invalid dice") {
        data.str = "d0";
        REQUIRE_NOTHROW(errors = data.validate());
        REQUIRE_FALSE(errors.ok());

        data.str = "d-1";
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
